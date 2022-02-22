# simulacrum

A modern library for procedural content generation

![random-maps](resources/all-maps.png)

# Example usage

```cpp

// math and algorithms
#include <simulacrum/random_walks.hpp>
#include <random>
#include <algorithm>

// data types
#include <glm/glm.hpp>
#include <cstdint>

// i/o
#include <iostream>

namespace ranges = std::ranges;
namespace cardinal = simulacrum::cardinal;

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

    // start at the origin of the grid
    glm::ivec2 const origin(0, 0);
    cardinal::walker homer(origin);

    // reserve enough space for all the points
    std::uint32_t const N = 5;
    std::vector<glm::ivec2> points;
    points.reserve(N);

    // -- perform the simulation --
    auto into_points = std::back_inserter(points);
    ranges::generate_n(into_points, N, cardinal::uniform_walk(rng, homer));
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

At this point, if you're using cmake, you can add this to your `CMakeLists.txt`

```cmake
find_package(simulacrum REQUIRED)
...
target_link_libraries(<target> INTERFACE sim::simulacrom)
```

### Windows Notes

If you're on windows, you'll need to run `cmake --install .` as adminsitrator
_instead_ of using `sudo`.

Also, as of right now (February 2022), there seems to be a bug with CMake on
Windows. The generated compile command likely won't include the necessary paths.
In order to fix this, you can add this to your cmake file:

```cmake
find_package(spatula REQUIRED)
get_target_property(simulacrum_includes sim::simulacrum INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(spatula_includes sp::spatula INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(<target> PRIVATE ${simulacrum_includes} ${spatula_includes})
```

# Documentation

## Overview
- [class walker](#class-walker)
    - [constructors](#walker-constructors)
    - [`walker::position()`](#walkerposition)
    - [`walker::step()`](#walkerstep)
- [simulation](#simulation)
    - [`cardinal::uniform_walk(rng, start)`](#cardinaluniform_walkrng-start)

## class walker
An abstract data type for simulating random walks.

### Signature
```cpp
template<sp::vector2 Vector> class walker;
```

## walker constructors

```cpp
/** Create a random walker at the origin. */
template<sp::vector2 Vector>
walker<Vector>::walker();

/** create a random walker at a specific point. */
template<sp::vector2 Vector>
walker<Vector>::walker(int x, int y);
template<sp::vector2 Vector>              // prefer this constructor for
walker<Vector>::walker(Vector const & p); // type-deduction
```

### Examples
```cpp
#include <SFML/System.hpp>
#include <simulacrum/random_walks.hpp>
using namespace simulacrum;

// ...

walker<sf::Vector2i> w1;            // walker at the origin
walker<sf::Vector2i> w2(100, 100);  // walker at (100, 100)

sf::Vector2i const start(100, 100);
walker w3(start);                   // walker at (100, 100)
```

## walker methods

## walker::position()
The walker's current position

### Signature
```cpp
template<sp::vector2 Vector>
Vector const & walker<Vector>::postion() const;
```

## walker::step()
Step the walker by one unit in the given direction.

### Signature
```cpp
template<vector2 Vector>
Vector const & walker<Vector>::step(cardinal::direction_name direction);
```

### Return
The walker's position after stepping.

### Parameters
- `direction` the cardinal direction to walk in


## simulation

## cardinal::uniform_walk(rng, start)
Make a function that generates points in a uniform-random walk.

### Signature
```cpp
template<std::uniform_random_bit_generator Engine, sp::vector2 Vector>
auto cardinal::uniform_walk(Engine & rng, Vector const & start);
```

### Return
A function that takes no arguments and returns a 2d point that's one step away
from the last point generated in a uniformly-distributed random cardinal
direction (North, East, South, West).

### Parameters
- `rng` the random number generator to sample from
- `start` where the random walk should start from

### Examples
```cpp
#include <glm/glm.hpp>
#include <simulacrum/random_walks.hpp>
namespace cardinal = simulacrum::cardinal;

// ...

std::random_device seed;
std::mt19937 rng(seed());

glm::ivec2 const origin(0, 0);
std::array<glm::ivec2, 10> points;
ranges::generate(points, cardinal::uniform_walk(rng, origin));
```
