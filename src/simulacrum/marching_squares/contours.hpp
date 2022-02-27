#pragma once

// data types and type constraints
#include <cstdint>
#include <spatula/geometry.hpp>
#include <ranges>
#include <iterator>

// data structures
#include <utility>
#include <bitset>
#include <vector>
#include <unordered_map>

// algorithms
#include <algorithm>

namespace simulacrum::marching_squares {

namespace ranges = std::ranges;

namespace point {
template<sp::semi_vector2 Vector>
Vector above(Vector const & v) { return Vector(v.x, v.y+1); }
template<sp::semi_vector2 Vector>
Vector below(Vector const & v) { return Vector(v.x, v.y-1); }
template<sp::semi_vector2 Vector>
Vector left(Vector const & v) { return Vector(v.x-1, v.y); }
template<sp::semi_vector2 Vector>
Vector right(Vector const & v) { return Vector(v.x+1, v.y); }
}

namespace cell {

using mask = std::bitset<4>;
inline mask constexpr lower_left{0b1};
inline mask constexpr upper_right{0b10};
inline mask constexpr upper_left{0b100};
inline mask constexpr lower_right{0b1000};

/** Compute the cell mask representing a cell's corners. */
template<ranges::input_range Range, sp::semi_vector2 Vector>
    requires std::same_as<ranges::range_value_t<Range>, Vector>

mask corner_mask(Range && points, Vector const & lower_left_corner)
{
    mask bits;

    Vector const upper_left_corner = point::above(lower_left_corner);
    if (ranges::find(points, lower_left_corner) != ranges::end(points)) {
        bits |= upper_left;
    }
    if (ranges::find(points, point::right(upper_left_corner)) != ranges::end(points)) {
        bits |= upper_right;
    }
    if (ranges::find(points, point::right(lower_left_corner)) != ranges::end(points)) {
        bits |= lower_right;
    }
    if (ranges::find(points, upper_left_corner) != ranges::end(points)) {
        bits |= lower_left;
    }
    return bits;
}

template<ranges::input_range PointRange>
    requires sp::semi_vector2<ranges::range_value_t<PointRange>>
auto count_corners(PointRange && points)
{
    using Point = ranges::range_value_t<PointRange>;
    return [&points](Point const & p) {
        return std::make_pair(p, cell::corner_mask(points, p));
    };
}
}

namespace vertex {

/** Get mesh-cell uv positions. */
template<sp::semi_vector2 Vertex>
static Vertex const lower_left() { return Vertex(0.f, 0.f); }
template<sp::semi_vector2 Vertex>
static Vertex const center_left() { return Vertex(0.f, .5f); }
template<sp::semi_vector2 Vertex>
static Vertex const upper_left() { return Vertex(0.f, 1.f); }
template<sp::semi_vector2 Vertex>
static Vertex const upper_center() { return Vertex(.5f, 1.f); }
template<sp::semi_vector2 Vertex>
static Vertex const upper_right() { return Vertex(1.f, 1.f); }
template<sp::semi_vector2 Vertex>
static Vertex const center_right() { return Vertex(1.f, .5f); }
template<sp::semi_vector2 Vertex>
static Vertex const lower_right() { return Vertex(1.f, 0.f); }
template<sp::semi_vector2 Vertex>
static Vertex const lower_center() { return Vertex(.5f, 0.f); }

}

namespace cell {

template<sp::semi_vector2 Vertex>
using mesh = std::vector<Vertex>;

// map cells to their respective contours - always moving clockwise
template<sp::semi_vector2 Vector>
mesh<Vector> const & contour_as(mask const & bits)
{
    static std::unordered_map<mask, mesh<Vector>> const contours{
        {mask(0b0), mesh<Vector>{}},

        // 1-point cses
        {mask(0b1),
         mesh({vertex::lower_left<Vector>(), vertex::center_left<Vector>(),
               vertex::lower_center<Vector>()})},

        {mask(0b10),
         mesh({vertex::lower_center<Vector>(), vertex::center_right<Vector>(),
               vertex::lower_right<Vector>()})},

        {mask(0b100),
         mesh({vertex::upper_center<Vector>(), vertex::upper_right<Vector>(),
               vertex::center_right<Vector>()})},

        {mask(0b1000),
         mesh({vertex::center_left<Vector>(), vertex::upper_left<Vector>(),
               vertex::upper_center<Vector>()})},

        // 2-point cases
        {mask(0b11),
         mesh({vertex::lower_left<Vector>(), vertex::center_left<Vector>(),
               vertex::center_right<Vector>(), vertex::lower_right<Vector>()})},

        {mask(0b101),
         mesh({vertex::lower_left<Vector>(), vertex::center_left<Vector>(),
               vertex::upper_center<Vector>(), vertex::upper_right<Vector>(),
              vertex::center_right<Vector>(), vertex::lower_center<Vector>()})},

        {mask(0b110),
         mesh({vertex::lower_center<Vector>(), vertex::upper_center<Vector>(),
               vertex::upper_right<Vector>(), vertex::lower_right<Vector>()})},

        {mask(0b1001),
         mesh({vertex::lower_left<Vector>(), vertex::upper_left<Vector>(),
               vertex::upper_center<Vector>(), vertex::lower_center<Vector>()})},

        {mask(0b1100),
         mesh({vertex::center_left<Vector>(), vertex::upper_left<Vector>(),
               vertex::upper_right<Vector>(), vertex::center_right<Vector>()})},

        {mask(0b1010),
         mesh({vertex::lower_center<Vector>(), vertex::center_left<Vector>(),
               vertex::upper_left<Vector>(), vertex::upper_center<Vector>(),
              vertex::center_right<Vector>(), vertex::lower_right<Vector>()})},

        // 3-point cases
        {mask(0b111),
         mesh({vertex::lower_left<Vector>(), vertex::center_left<Vector>(),
               vertex::upper_center<Vector>(), vertex::upper_right<Vector>(),
              vertex::lower_right<Vector>()})},

        {mask(0b1011),
         mesh({vertex::lower_left<Vector>(), vertex::upper_left<Vector>(),
               vertex::upper_center<Vector>(), vertex::center_right<Vector>(),
              vertex::lower_left<Vector>()})},

        {mask(0b1101),
         mesh({vertex::lower_left<Vector>(), vertex::upper_left<Vector>(),
               vertex::upper_right<Vector>(), vertex::center_right<Vector>(),
              vertex::lower_center<Vector>()})},

        {mask(0b1110),
         mesh({vertex::center_left<Vector>(), vertex::upper_left<Vector>(),
               vertex::upper_right<Vector>(), vertex::lower_right<Vector>(),
              vertex::lower_center<Vector>()})},

        // 4-point cases
        {mask(0b1111),
         mesh({vertex::lower_left<Vector>(), vertex::upper_left<Vector>(),
               vertex::upper_right<Vector>(), vertex::lower_right<Vector>()})}
    };
    // all possible cell masks have been defined, so this will never fail
    return contours.find(bits)->second;
}

}
}