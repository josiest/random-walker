// abstract and pure data types
#include "point.hpp"
#include "walker.hpp"

// math and algorithms
#include <random>
#include <ranges>
#include "simulation.hpp"

// i/o
#include <iostream>

void print(point const & p)
{
    std::cout << "{" << p.x << ", " << p.y << "}" << std::endl;
}

int main()
{
    // resources for the walk
    std::random_device seed;
    std::mt19937 rng(seed());
    walker homer; // homer will tell the simulation where he is at each step

    // perform walk then print points
    pointset points = walk(rng, homer, 100);
    ranges::for_each(points, print);
}
