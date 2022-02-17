#pragma once

// data structures
#include <unordered_set>

// abstract and pure data types
#include "point.hpp"
#include "direction.hpp"
#include "walker.hpp"
#include <cstdint>

// math and algorithms
#include <random>
#include <algorithm>
#include <ranges>

namespace random_walk {

// aliases and namespaces
using pointset = std::unordered_set<point>;
namespace ranges = std::ranges;

template<std::uniform_random_bit_generator engine_t>
pointset walk(engine_t & rng, walker & homer, std::uint32_t N)
{
    // create the resources for the random walk
    direction_distribution random_direction(0, direction::size-1);
    pointset points;
    points.reserve(N);

    // have a random walker take a step in a random direction
    auto next_step = [&rng, &homer, &random_direction]() {
        auto const dir = static_cast<direction::name>(random_direction(rng));
        homer.step(dir);
        return homer.position();
    };

    // perform the random walk
    auto into_points = std::inserter(points, points.begin());
    ranges::generate_n(into_points, N, next_step);
    return points;
}
}