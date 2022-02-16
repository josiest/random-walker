#pragma once

#include <functional>

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

namespace std {
template<>
struct hash<point> {
    
    size_t operator()(const point & p) const
    {
        return p.x ^ (p.y << 1);
    }
};
}

inline bool operator==(point const & lhs, point const & rhs)
{
    return lhs.x == rhs.x and lhs.y == rhs.y;
}
