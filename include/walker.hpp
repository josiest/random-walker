#pragma once

#include <array>

// a basic 2d spatial data type
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


// the possible direction types
namespace direction { enum name { up, left, down, right, size }; };

// the directions as vectors
inline std::array<point, 4> constexpr directions{
    point{0, -1}, point{-1, 0}, point{0, 1}, point{1, 0}
};

// an adt for walking aimlessly!
class walker {
public:
    // create a random walker at the origin
    inline walker() : _position{0, 0} {}

    // create a random walker at a specific point
    inline walker(int x, int y) : _position{x, y} {}

    // the walker's current position
    inline point const & position() const { return _position; }

    // step in the given direction
    inline void step(direction::name dir)
    {
        std::size_t const i = static_cast<std::size_t>(dir);
        _position += directions[i];
    }
private:
    point _position;
};
