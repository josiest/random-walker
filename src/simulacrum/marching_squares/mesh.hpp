#pragma once

// data types and data structures
#include <cstddef>
#include <vector>

// type constraints
#include <spatula/geometry.hpp>
#include <iterator>
#include <ranges>

// math and algorithms
#include "simulacrum/marching_squares/contours.hpp"
#include <algorithm>
#include <numeric>

namespace simulacrum::marching_squares {

namespace views = std::views;

// return a function that offsets a vertex by a constant offset
template<sp::semi_vector2 Vector>
auto offset_by_vector(Vector const & offset)
{
    return [&offset](Vector const & v) {
        return Vector(v.x + offset.x, v.y + offset.y);
    };
}

// return a function that inserts cell vertices into a mesh
template<sp::semi_vector2 Vertex, std::weakly_incrementable VertexInserter>
    requires std::indirectly_writable<VertexInserter, Vertex>

auto triangulate_cell(VertexInserter into_verts)
{
    using VertexMapping = std::pair<Vertex, cell::mask>;
    return [into_verts](VertexMapping const & mapping) {

        auto const &[lower_left, mask] = mapping;
        auto const & contour = cell::contour_as<Vertex>(mask);

        // insert each vertex in the cell offset by the current lower-left
        ranges::transform(contour, into_verts, offset_by_vector(lower_left));
    };
}

/** Triangulate a set of points into a mesh.
 * 
 * Parameters
 *   points - the points to turn into a mesh
 *   into_indices - where to write the mesh-cell start indices
 *   into_counts - where to write the mesh-cell vertex counts
 *   into_verts - where to write the mesh vertices
 * 
 * Notes
 *   verts will be filled in triangle-fan order with indices and counts as
 *   specified by glMultiDrawArrays
 */
template<sp::semi_vector2 Vertex,
         ranges::input_range PointRange,
         std::weakly_incrementable IndexInserter,
         std::weakly_incrementable CountInserter,
         std::weakly_incrementable VertexInserter>

    requires sp::semi_vector2<ranges::range_value_t<PointRange>> and
             std::indirectly_writable<IndexInserter, std::size_t> and
             std::indirectly_writable<CountInserter, std::size_t> and
             std::indirectly_writable<VertexInserter, Vertex>

void triangulate(PointRange && points, IndexInserter into_indices,
                 CountInserter into_counts, VertexInserter into_verts)
{
    // alias some useful types
    using Point = ranges::range_value_t<PointRange>;
    using PointMapping = std::pair<Point, cell::mask>;
    using VertexMapping = std::pair<Vertex, cell::mask>;

    // a temporary list of vertex-mask pairs for reference
    std::vector<VertexMapping> vertex_map;
    vertex_map.reserve(ranges::size(points));
    auto into_vertex_map = std::back_inserter(vertex_map);

    auto as_vertex_mapping = [](PointMapping const & pair) {
        return std::make_pair(static_cast<Vertex>(pair.first), pair.second);
    };
    ranges::transform(points, into_vertex_map, as_vertex_mapping,
                      cell::count_corners(points));

    // temporary list of vertex counts for reference
    std::vector<std::uint32_t> cell_counts;
    cell_counts.reserve(ranges::size(points));
    auto into_cell_counts = std::back_inserter(cell_counts);

    ranges::transform(vertex_map, into_cell_counts, &cell::mask::count,
                      &VertexMapping::second);

    // generate the mesh-cell indices and their vertex counts
    std::exclusive_scan(cell_counts.begin(), cell_counts.end(),
                        into_indices, 0);
    ranges::copy(cell_counts, into_counts);

    // compute the vertices
    ranges::for_each(vertex_map, triangulate_cell<Vertex>(into_verts));
}

}