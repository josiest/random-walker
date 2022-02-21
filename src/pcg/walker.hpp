#pragma once

// data types
#include <spatula/vectors.hpp>
#include "pcg/direction.hpp"

namespace pcg::cardinal {

// an adt for walking aimlessly!
template<spatula::vector2 Vector>
class walker {
public:
    // create a random walker at the origin
    inline walker() : _position{0, 0} {}

    // create a random walker at a specific point
    inline walker(int x, int y) : _position{x, y} {}

    // create a random walker from a point object
    inline walker(Vector const & p) : _position(p) {}

    // the walker's current position
    inline Vector const & position() const { return _position; }

    // step in the given direction
    inline Vector const & step(cardinal::direction_name direction)
    {
        _position += cardinal::direction_as<Vector>(direction);
        return _position;
    }
private:
    Vector _position;
};
}