#pragma once

// data types
#include "point.hpp"
#include "direction.hpp"

namespace random_walk {

// an adt for walking aimlessly!
template<spatial2 point_t = point>
class walker {
public:
    // create a random walker at the origin
    inline walker() : _position{0, 0} {}

    // create a random walker at a specific point
    inline walker(int x, int y) : _position{x, y} {}

    // create a random walker from a point object
    inline walker(point_t const & p) : _position(p) {}

    // the walker's current position
    inline point_t const & position() const { return _position; }

    // step in the given direction
    inline void step(direction::name dir)
    {
        std::size_t const i = static_cast<std::size_t>(dir);
        _position += directions[i];
    }
private:
    point_t _position;
};
}