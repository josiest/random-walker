// math and algorithms
#include <random>
#include <pcg/random_walk.hpp>
#include <ranges>
#include <iterator>

// i/o
#include <iostream>

// aliases and namespaces
namespace ranges = std::ranges;
namespace cardinal = pcg::cardinal;

void print(pcg::point const & p)
{
    std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
}

int main(int argc, char * argv[])
{
    // Parse the user input for the number of steps to perform
    int N = 100;
    if (argc == 2) { try {
        N = std::stoi(argv[1]);
    }
    // fail if any error occured while converting th N-argument to int
    catch (...) {
        std::cout << "couldn't convert \"" << argv[1] << "\" to int" << std::endl;
        return EXIT_FAILURE;
    }}
    // fail if the wrong number of arguments were provided
    else if (argc > 2) {
        std::cout << "usage: walk [N]" << std::endl
                  << " N - number of steps to take" << std::endl;
        return EXIT_FAILURE;
    }

    // resources for the walk
    std::random_device seed;
    std::mt19937 rng(seed());

    pcg::point const origin(0, 0);

    // points will be written from the walk into this point set
    pcg::pointset points;
    points.reserve(N);
    auto into_points = std::inserter(points, points.begin());

    // perform walk then print points
    ranges::generate_n(into_points, N, cardinal::uniform_walk(rng, origin));
    ranges::for_each(points, print);
    return EXIT_SUCCESS;
}
