#pragma once

#include <functional>

namespace random_walk {
// a basic 1d spatial data type
struct point {
    int x, y;
    inline point & operator+=(point const & rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
};

inline bool operator==(point const & lhs, point const & rhs)
{
    return lhs.x == rhs.x and lhs.y == rhs.y;
}
}

namespace std {
template<>
struct hash<random_walk::point> {
    
    size_t operator()(const random_walk::point & p) const
    {
        return p.x ^ (p.y << 1);
    }
};
}