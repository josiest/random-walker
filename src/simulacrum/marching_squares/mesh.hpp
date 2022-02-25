#pragma once

// data structures and abstract data types
#include <spatula/geometry.hpp>
#include "simulacrum/marching_squares/contours.hpp"
#include <vector>

// algorithms
#include <algorithm>
#include <numeric>

namespace simulacrum::marching_squares {

/** An output iterator that writes unsigned integers. */
template<class Inserter>
concept unsigned_inserter = requires
     std::weakly_incrementable<Inserter> and
     std::unsigned_integral<std::iter_value_t<Inserter>> and
     std::indirectly_writable<Inserter, std::iter_value_t<Inserter>>;

// return a function that offsets a vertex by a constant offset
template<sp::vector2 Vector>
Vector offset_by_vector(Vector const && lower_left)
{
    return [&lower_left](Vector && cell_vertex) {
        return lower_left + cell_vertex;
    };
}

// return a function that inserts cell vertices into a mesh
template<sp::vector2_inserter VertexInserter>
void triangulate_cell(VertexInserter into_verts)
{
    using Vertex = std::iter_value_t<VertexInserter>;
    using VertexMapping = std::pair<Vertex, cell::mask>;
    return [into_verts](VertexMapping const & pair) {

        auto const &[lower_left, mask] = pair;
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
template<sp::vector2_irange PointRange, unsigned_inserter IndexInserter,
         unsigned_inserter CountInserter, sp::vector2_inserter VertexInserter>

void triangulate(PointRange && points, IndexInserter into_indices,
                 CountInserter into_counts, VertexInserter into_verts)
{
    // alias some useful types
    using Vertex = std::iter_value_t<VertexInserter>;
    using VertexMapping = std::pair<Vertex, cell::mask>;

    // a temporary list of vertex-mask pairs for reference
    std::vector<VertexMapping> vertex_map;
    vertex_map.reserve(ranges::size(points));
    auto into_vertex_map = std::back_inserter(vertex_map);

    auto as_vertex_mapping = [](auto const & pair) {
        return std::make_pair(static_cast<Vertex>(pair.first), pair.second);
    }
    ranges::transform(points, into_vertex_map,
                      cell::count_corners(points), as_vertex_mapping);

    // temporary list of vertex counts for reference
    std::vector<std::uint32_t> cell_counts;
    cell_counts.reserve(ranges::size(points));
    auto into_cell_counts = std::back_inserter(counts);

    ranges::transform(vertex_map, into_cell_counts,
                      &VertexMapping::second, &cell::mask::count);

    // get the mesh-cell indices with their vertex counts
    std::exclusive_scan(cell_counts.begin(), cell_counts.end(),
                        into_indices, 0);
    ranges::copy(cell_counts, into_counts);

    // compute the vertices
    ranges::for_each(vertex_map, triangulate_cell(into_verts));
}

}