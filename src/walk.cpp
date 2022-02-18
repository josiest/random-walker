// abstract and pure data types
#include "point.hpp"
#include "walker.hpp"

// math and algorithms
#include <random>
#include <ranges>
#include "simulation.hpp"

// i/o
#include <iostream>
#include <string>

// aliases and namespaces
namespace rw = random_walk;
namespace ranges = std::ranges;

void print(rw::point const & p)
{
    std::cout << "{" << p.x << ", " << p.y << "}" << std::endl;
}

int main(int argc, char * argv[])
{
    int N = 100;
    if (argc == 2) { try {
        N = std::stoi(argv[1]);
    }
    catch (...) {
        std::cout << "couldn't convert \"" << argv[1] << "\" to int" << std::endl;
        return EXIT_FAILURE;
    }}
    else if (argc > 2) {
        std::cout << "usage: walk [N]" << std::endl
                  << " N - number of steps to take" << std::endl;
        return EXIT_FAILURE;
    }

    // resources for the walk
    std::random_device seed;
    std::mt19937 rng(seed());

    // homer will tell the simulation where he is at each step
    rw::walker homer(rw::point::origin());

    // perform walk then print points
    rw::pointset<rw::point> points = rw::walk(rng, homer, N);
    ranges::for_each(points, print);
    return EXIT_SUCCESS;
}
