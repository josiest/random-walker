// data structures
#include <vector>
#include <unordered_set>

// math and algorithms
#include <random>
#include <ranges>
#include <algorithm>

// data types
#include "walker.hpp"
#include <cstddef>

// i/o
#include <iostream>

// aliases and namespaces
namespace ranges = std::ranges;
namespace views = std::views;
using direction_distribution = std::uniform_int_distribution<std::size_t>;

// get a referece to the global random engine
std::mt19937 & rng()
{
    static std::mt19937 rng(std::random_device{}());
    return rng;
}

// have a random walker take a step in a random direction
// returns a function that can be used with ranges::generate_n
auto next_step(walker & homer, direction_distribution & random_direction)
{
    return [&]() {
        auto const dir = static_cast<direction::name>(random_direction(rng()));
        homer.step(dir);
        return homer.position();
    };
}

void print(point const & p)
{
    std::cout << "{" << p.x << ", " << p.y << "}" << std::endl;
}

int main()
{
    std::size_t const N = 1000;

    // create the resources for the random walk
    direction_distribution random_direction(0, direction::size-1);
    walker homer; // homer will tell us his position at each step

    std::unordered_set<point> points;
    points.reserve(N);

    // perform the random walk and print the generated points
    auto into_points = std::inserter(points, points.begin());
    ranges::generate_n(into_points, N, next_step(homer, random_direction));
    ranges::for_each(points, print);
}
