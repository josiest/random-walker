#pragma once

// abstract and pure data types
#include "pcg/point.hpp"
#include "pcg/direction.hpp"
#include "pcg/walker.hpp"
#include <cstdint>

// math and algorithms
#include <random>
#include <algorithm>
#include <ranges>

namespace pcg {

// aliases and namespaces
namespace ranges = std::ranges;

namespace cardinal {
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
template<vector2 Vector>
auto uniform_walk(std::uniform_random_bit_generator auto & rng,
                  walker<Vector> & homer)
{
    // return a generator function that will step in a random cardinal direction
    return [&rng, &homer]() {
        return homer.step(uniform_enum<cardinal::direction_name>(rng));
    };
}

}
}