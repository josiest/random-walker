#pragma once

#include <unordered_set>
#include <type_traits>
#include <concepts>

namespace random_walk {

template<class point_t>
concept spatial2 = requires(point_t p) {
    p.x and std::is_arithmetic_v<std::remove_reference_t<decltype(p.x)>>;
    p.y and std::is_arithmetic_v<std::remove_reference_t<decltype(p.y)>>;
    std::is_same_v<decltype(p.x), decltype(p.y)>;
    point_t(p.x, p.y);
    std::copy_constructible<point_t>;
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

template<spatial2 point_t>
using pointset = std::unordered_set<point_t>;

// a basic 1d spatial data type
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