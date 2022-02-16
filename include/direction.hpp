#pragma once

// data structures and data types
#include <array>
#include "point.hpp"
#include <cstddef>

// math
#include <random>

// the possible direction types
namespace direction { enum name { up, left, down, right, size }; };

// the directions as vectors
inline std::array<point, 4> constexpr directions{
    point{0, -1}, point{-1, 0}, point{0, 1}, point{1, 0}
};

using direction_distribution = std::uniform_int_distribution<std::size_t>;