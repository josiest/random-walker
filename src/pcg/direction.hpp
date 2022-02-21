#pragma once

// type constraints
#include <type_traits>

// data structures and data types
#include "pcg/point.hpp"
#include <array>
#include <cstdint>
#include <cstddef>

// math
#include <random>

namespace pcg {

/** An enum with sequential values defined from [0, Enum::size). */
template<enum Enum>
concept ranged_enum = std::is_enum_v<Enum> and requires { Enum::size; };

namespace cardinal {

/** The cardinal directions. */
enum direction_name { north, east, south, west, size };

/** Get the vector associated with a cardinal direction. */
template<spatula::vector2 Vector>
constexpr Vector const &
direction_as(direction_name dir)
{
    static std::array<Vector, direction_name::size> directions{
        /* north */ Vector(0, 1), /* east */ Vector(1, 0),
        /* south */ Vector(0, -1), /* west */ Vector(-1, 0)
    };
    return directions[dir];
};

}

/** Uniformly sample a random direction. */
template<ranged_enum Enum>
Enum uniform_enum(std::uniform_random_bit_generator auto & rng)
{
    using enum_distribution = std::uniform_int_distribution<std::uint32_t>;
    static enum_distribution sample_enum(0, Enum::size);

    return static_cast<Enum>(sample_enum(rng));
}

}