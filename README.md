# random walks

A modern library for a classic algorithm. Simulate random walks easily.

# Example usage

```cpp
#include <pcg/random_walk.hpp>
#include <random>
#include <algorithm>
#include <iterator>
#include <cstdint>
#include <iostream>

namespace ranges = std::ranges;
namespace cardinal = pcg::cardinal;

void print_point(pcg::point const & p)
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
    pcg::point const origin(0, 0);

    // reserve enough space for all the points
    std::uint32_t const N = 10;
    pcg::pointset points;
    points.reserve(N);

    // -- perform the simulation --
    auto into_points = std::inserter(points, points.begin());
    ranges::generate_n(into_points, N, cardinal::uniform_walk(rng, origin));
    ranges::for_each(points, print_point);
}
```

Possible output:

```
(1, -2)
(-1, -2)
(0, -1)
(0, 0)
(1, -1)
(0, -2)
(1, 0)
```

# Requirements
- A compiler for C++20 or newer

# Installation
Since this is a header-only library, all you _really_ need to do is to make sure
that the compiler can see the include path that has the source code.
Alternatively, you can build it with cmake:

```sh
mkdir build && cd build
cmake ..
cmake --build .
sudo cmake --install .
```

If you're on windows, you'll need to run `cmake --install .` as adminsitrator
_instead_ of using `sudo`.

# Documentation

## struct point
A 2-dimensional point data structure.

### Members
`int x, y` the (x, y) coordinates

### Defined Operations
- addition, equality, hashing

### Examples
```cpp
using namespace pcg;

point const a(1, 2);
point const b(3, 4);
point const c = a + b;
assert(c == point(4, 6));

std::unordered_set<point> points{a, b, c};
```

## concept pcg::vector2
A 2-dimensional vector concept.
 
### Requirements
A vector2 type must satisfy the following requirements
- copy constructible, and constructible from two numeric values
- has public numeric members x and y with the same type

### Examples
```cpp
auto dot(pcg::vector2 auto const & a, pcg::vector2 auto const & b)
{
    // use a's field type (as opposed to potentially b's instead)
    using Field = std::remove_reference_t<decltype(a.x)>;
    return static_cast<Field>(a.x*b.x + a.y*b.y);
}
```

## auto pcg::cardinal::uniform_walk(rng, start)
Make a function that generates points in a uniform-random walk.

### Signature
```cpp
template<std::uniform_random_bit_generator Engine, vector2 Vector>
auto pcg::cardinal::uniform_walk(Engine & rng, Vector const & start);
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
std::random_device seed;
std::mt19937 rng(seed());

pcg::point const origin(0, 0);
std::array<pcg::point, 10> points;
ranges::generate(points, pcg::cardinal::uniform_walk(rng, origin));
```