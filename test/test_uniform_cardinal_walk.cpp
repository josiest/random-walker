#include <catch2/catch_test_macros.hpp>
#include "pcg/random_walk.hpp"

#include <random>
#include <vector>
#include <algorithm>

namespace ranges = std::ranges;
using namespace pcg;

struct pointf { float x; float y; };
pointf operator+(pointf const & a, pointf const & b)
{
    return pointf(a.x + b.x, a.y + b.y);
}

TEST_CASE("walk-size: empty", "[single-file]")
{
    std::mt19937 rng;
    point origin(0, 0);

    std::vector<point> points;
    auto into_points = std::back_inserter(points);
    ranges::generate_n(into_points, 0, cardinal::uniform_walk(rng, origin));

    REQUIRE(points.empty());
}

TEST_CASE("walk-size: one", "[single-file]")
{
    std::mt19937 rng;
    point origin(0, 0);

    std::vector<point> points;
    auto into_points = std::back_inserter(points);
    ranges::generate_n(into_points, 1, cardinal::uniform_walk(rng, origin));

    REQUIRE(points.size() == 1);
}

TEST_CASE("walk-size: many", "[single-file]")
{
    std::mt19937 rng;
    point origin(0, 0);

    std::vector<point> points;
    auto into_points = std::back_inserter(points);
    ranges::generate_n(into_points, 200, cardinal::uniform_walk(rng, origin));

    REQUIRE(points.size() == 200);
}

// TEST_CASE("adjacency: two", "[single-file]")
// {
// 
// }
// 
// TEST_CASE("adjacency: many", "[single-file]")
// {
// 
// }
// 
// TEST_CASE("generic: float-point", "[single-file]")
// {
// 
// }
// 
// TEST_CASE("start: positive components", "[single-file]")
// {
// 
// }
// 
// TEST_CASE("start: negative components" "[single-file]")
// {
// 
// }
// 