#pragma once

// abstract and pure data types
#include <spatula/geometry.hpp>
#include "simulacrum/random_walks/walker.hpp"
#include <cstdint>

// math and algorithms
#include <random>
#include <algorithm>
#include <ranges>

namespace simulacrum {

/** Uniformly sample a random direction. */
template<sp::ranged_enum Enum>
Enum uniform_enum(std::uniform_random_bit_generator auto & rng)
{
    using enum_distribution = std::uniform_int_distribution<std::uint32_t>;
    static enum_distribution sample_enum(0, Enum::size-1);

    return static_cast<Enum>(sample_enum(rng));
}

}

namespace simulacrum::cardinal {

// aliases and namespaces
namespace ranges = std::ranges;

/** Create a function that walks aimlessly in a 2d plane.
 * 
 * Return
 *  A function that takes no arguments and returns a 2d point that's one step
 *  away from the last point generated in a uniformly-distributed random
 *  cardinal direction (left, right, up, down).
 * 
 * Parameters
 *  rng - the random number generator to sample from
 *  start - where the random walk should start from
 */
template<sp::vector2 Vector>
auto uniform_walk(std::uniform_random_bit_generator auto & rng,
                  walker<Vector> & homer)
{
    using uniform_direction = uniform_enum<sp::cardinal::direction_name>;
    // return a generator function that will step in a random cardinal direction
    return [&rng, &homer]() {
        return homer.step(uniform_direction(rng));
    };
}

}