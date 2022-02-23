#pragma once

#include <spatula/geometry.hpp>
#include <cstdint>
#include <random>

namespace simulacrum {

/** Simulate a random walk.
 * 
 * Return
 *   A function object that takes no arguments and advances a unit-step in a
 *   random direction when called, returning its previous position.
 */
template<sp::ranged_enum Direction,
         std::uniform_random_bit_generator Engine,
         sp::vector2 Vector,
         class Distribution>

class random_walk {
public:

    /** Step by one unit in a random direction
     * 
     * Return
     *   The position after stepping.
     */
    random_walk(Engine & rng, Vector start, Distribution sample_space)
        : _rng(rng), _sample_direction(sample_space), _position(start) {}

    Vector operator()()
    {
        // save the old position to return
        Vector old_position = _position;

        // sample a random direction and walk
        auto const direction = static_cast<Direction>(_sample_direction(_rng));
        sp::direction_as<Direction, Vector> to_vector;
        _position = _position + to_vector(direction);

        return old_position;
    }
private:
    Engine & _rng;
    Vector _position;
    Distribution _sample_direction;
};

/** Simulate a uniform random walk.
 * 
 * Return
 *   A random_walk function object.
 */
template<sp::ranged_enum Direction,
         std::uniform_random_bit_generator Engine,
         sp::vector2 Vector>
         
auto uniform_walk(Engine & rng, Vector start)
{
    using direction_distribution = std::uniform_int_distribution<std::uint32_t>;
    using UniformWalk = random_walk<Direction, Engine,
                                    Vector, direction_distribution>;

    direction_distribution uniform_direction(0, Direction::size-1);
    return UniformWalk(rng, start, uniform_direction);
}
}