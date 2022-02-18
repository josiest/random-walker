#pragma once

#include <unordered_set>
#include <type_traits>
#include <concepts>

namespace random_walk {

/** A spatial type of 2 dimensions.
 * 
 * A spatial2 type must satisfy the following requirements
 * - copy constructible, and constructible from two numeric values
 * - has public numeric members x and y with the same type
 * - can increment itself by another object of the same type
 *
 * From these requirements, the following operations have been defined
 * - equality, hashing, addition
 */
template<class point_t>
concept spatial2 = requires(point_t p) {

    // -- member requirements --
    // point_t has an x member that's numeric
    p.x and std::is_arithmetic_v<std::remove_reference_t<decltype(p.x)>>;
    // point_t has a y member with the same type as x
    p.y and std::is_same_v<decltype(p.x), decltype(p.y)>;

    // -- constructor requirements --
    // point_t is constructable from two numeric values
    point_t(p.x, p.y);
    std::copy_constructible<point_t>;

    // -- operator requirements --
    { p += p } -> std::same_as<point_t&>;
};

template<spatial2 point_a, spatial2 point_b>
point_a operator+(point_a const & a, point_b const & b)
{
    return point_a(a.x+b.x, a.y+b.y);
}

bool operator==(spatial2 auto const & lhs, spatial2 auto const & rhs)
{
    return lhs.x == rhs.x and lhs.y == rhs.y;
}
}

namespace std {
template<random_walk::spatial2 point_t>
struct hash<point_t> {
    
    size_t operator()(point_t const & p) const
    {
        return p.x ^ (p.y << 1);
    }
};
}

namespace random_walk {

/** An unordered set of spatial2 objects. */
template<spatial2 point_t>
using pointset = std::unordered_set<point_t>;

// a basic 2d spatial data type
struct point {
    int x, y;
    point & operator+=(point const & q)
    {
        x += q.x;
        y += q.y;
        return *this;
    }

    inline static point origin() { return point{0, 0}; }
};

}