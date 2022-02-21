#pragma once

// type constraints
#include <type_traits>
#include <concepts>

// data structures
#include <unordered_set>

namespace pcg {

/** A 2-dimensional vector.
 * 
 * A vector2 type must satisfy the following requirements
 * - copy constructible, and constructible from two numeric values
 * - has public numeric members x and y with the same type
 */
template<class Vector>
concept vector2 = requires(Vector p) {

    // -- member requirements --
    // Vector has an x member that's numeric
    p.x and std::is_arithmetic_v<std::remove_reference_t<decltype(p.x)>>;
    // Vector has a y member with the same type as x
    p.y and std::is_same_v<decltype(p.x), decltype(p.y)>;

    // -- constructor requirements --
    // Vector is constructable from two numeric values
    Vector(p.x, p.y);
    std::copy_constructible<Vector>;

    // -- operator requirements --
    { p + p } -> std::same_as<Vector>;
};

// a basic 2d spatial data type
struct point {
    int x, y;
    point() : x(0), y(0) {}
    point(int x, int y) : x(x), y(y) {}
    point & operator+=(point const & q)
    {
        x += q.x;
        y += q.y;
        return *this;
    }
};

point operator+(point const & p, point const & q)
{
    point s(p);
    s += q;
    return s;
}

bool operator==(point const & p, point const & q)
{
    return p.x == q.x and p.y == q.y;
}
}

namespace std {
template<>
struct hash<pcg::point> {
    size_t operator()(pcg::point const & p) const
    {
        using Field = decay_t<decltype(p.x)>;
        hash<Field> field_hash;
        return field_hash(p.x) ^ (field_hash(p.y) << 1);
    }
};
}

namespace pcg {
/** An unordered set of vector2 objects. */
using pointset = std::unordered_set<point>;
}
