#pragma once

// data types
#include "pcg/point.hpp"
#include "pcg/direction.hpp"

namespace pcg::cardinal {

/** An abstract data type for simulating random walks. */
template<vector2 Vector>
class walker {
public:
    /** Create a random walker at the origin. */
    inline walker() : _position{0, 0} {}

    /** create a random walker at a specific point. */
    inline walker(int x, int y) : _position{x, y} {}
    inline walker(Vector const & p) : _position(p) {}

    /** The walker's current position. */
    inline Vector const & position() const { return _position; }

    /**
     * Step the walker by one unit in the given direction.
     * 
     * Return
     *   The walker's position after stepping.
     * 
     * Parameters
     *   direction - the cardinal direction to walk in
     */
    inline Vector const & step(cardinal::direction_name direction)
    {
        _position = _position + cardinal::direction_as<Vector>(direction);
        return _position;
    }
private:
    Vector _position;
};
}