// math and algorithms
#include <simulacrum/random_walks.hpp>
#include <random>
#include <algorithm>

// data structures
#include <vector>

// i/o
#include <iostream>

// aliases and namespaces
namespace ranges = std::ranges;
namespace sim = simulacrum;

struct point{ int x; int y; };
// define the operations required for vector-closure
point operator+(point const & p, point const & q) { return { p.x + q.x, p.y + q.y }; }
point operator-(point const & p, point const & q) { return { p.x - q.x, p.y - q.y }; }
point operator*(int c, point const & p) { return { c * p.x, c * p.y }; }
bool operator==(point const & p, point const & q) { return p.x == q.x and p.y == q.y; }

void print(point const & p)
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
    point const origin(0, 0);

    // points will be written from the walk into this point set
    std::vector<point> points;
    points.reserve(N);
    auto into_points = std::back_inserter(points);

    // perform walk then print points
    using cardinal = sp::cardinal::direction_name;
    ranges::generate_n(into_points, N, sim::uniform_walk<cardinal>(rng, origin));
    ranges::for_each(points, print);
    return EXIT_SUCCESS;
}
