# simulacrum

A modern library for procedural content generation

![random-maps](resources/all-maps.png)

# Example usage

```cpp

// math and algorithms
#include <simulacrum/random_walks.hpp>
#include <random>
#include <algorithm>

// data types and type constraints
#include <glm/glm.hpp>
#include <cstdint>
#include <spatula/geometry.hpp>

// i/o
#include <iostream>

namespace ranges = std::ranges;
namespace sim = simulacrum;

void print_point(glm::ivec2 const & p)
{
    std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
}

int main()
{
    // -- create resources for the walk --

    // we'll need to make our own random engine
    std::random_device seed;
    std::mt19937 rng(seed());
    glm::ivec2 const origin(0, 0);

    // reserve enough space for all the points
    std::uint32_t const N = 5;
    std::vector<glm::ivec2> points;
    points.reserve(N);
    auto into_points = std::back_inserter(points);

    // -- perform the simulation --
    using cardinal = sp::direction::cardinal;
    ranges::generate_n(into_points, N, sim::uniform_walk<cardinal>(rng, origin));
    ranges::for_each(points, print_point);
}
```

Possible output:

```
(0, 1)
(1, 1)
(0, 1)
(0, 0)
(0, -1)
```

# Requirements
- A compiler for C++20 or newer
- [`spatula` library](https://github.com/josiest/spatula)

# Installation
Since this is a header-only library, all you _really_ need to do is to make sure
that the compiler can see the include path that has the source code.
Alternatively, you can build it with cmake:

```sh
mkdir build
cd build
cmake ..
sudo cmake --install .
```

If you're on windows, you'll need to run `cmake --install .` as adminsitrator
_instead_ of using `sudo`.

At this point, if you're using cmake, you can add this to your `CMakeLists.txt`

```cmake
find_package(simulacrum REQUIRED)
...
target_link_libraries(<target> INTERFACE sim::simulacrom)
```

# Documentation

## Overview
- [`random_walk(rng, start, sample_space)`](#random_walkrng-start-sample_space)
- [`uniform_walk(rng, start)`](#uniform_walkrng-start)

## simulation

## random_walk(rng, start, sample_space)
Simulate a random walk.

### Signature
```cpp
namespace simulacrum {
template<sp::ranged_enum Direction,
         std::uniform_random_bit_generator Engine,
         sp::vector Vector,
         class Distribution>

class random_walk {
public:
    random_walk(Engine & rng, Vector const & start,
                Distribution const & sample_space);
    Vector operator()();
};
}
```

### Return
A function object that takes no arguments and advances the walk's position,
returning the previous position.

### Parameters
- `rng` the random number generator
- `start` the starting position of the walk
- `sample_space` the direction distribution to use when walking

### Examples
```cpp
#include <random>
#include <SFML/System.hpp>
#include <spatula/geometry.hpp>
#include <simulacrum/random_walks.hpp>
// ...

std::random_device seed;
std::mt19937 rng(seed());
sf::Vector2i const start(100, 100);

using direction_distribution = std::discrete_distribution<std::uint32_t>;
direction_distribution sample_direction{3, 2, 1, 1};

std::uint32_t const N = 100;
std::vector<sf::Vector2i> points;
points.reserve(N);

auto into_points = std::back_inserter(points);
using cardinal = sp::direction::cardinal;
ranges::generate_n(into_points, N,
                   sim::random_walk<cardinal>(rng, start, sample_direction));
// ...
```

## uniform_walk(rng, start)
Simulate a uniform random walk

### Signature
```cpp
namespace simulacrum {
template<std::uniform_random_bit_generator Engine, sp::vector2 Vector>
auto uniform_walk(Engine & rng, Vector const & start);
}
```

### Return
A `random_walk` function object with a `std::uniform_int_distribution` as its
sample space.

### Parameters
- `rng` the random number generator to sample from
- `start` where the random walk should start from

### Examples
```cpp
#include <glm/glm.hpp>
#include <spatula/geometry.hpp>
#include <simulacrum/random_walks.hpp>

namespace sim = simulacrum;
// ...

std::random_device seed;
std::mt19937 rng(seed());
glm::ivec2 const origin(0, 0);

std::array<glm::ivec2, 10> points;
using cardinal = sp::cardinal::direction_name;
ranges::generate(points, sim::uniform_walk<cardinal>(rng, origin));
// ...
```
