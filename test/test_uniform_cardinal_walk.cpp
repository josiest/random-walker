#include <catch2/catch_test_macros.hpp>
#include "pcg/random_walk.hpp"

// math and algorithms
#include <random>
#include <cmath>
#include <algorithm>

// data structures and data types
#include <vector>
#include <cstdint>

namespace ranges = std::ranges;
namespace views = std::views;
using namespace pcg;

struct pointf { float x; float y; };
pointf operator+(pointf const & a, pointf const & b)
{
    return pointf(a.x + b.x, a.y + b.y);
}

int distance(point const & a, point const & b)
{
    return std::abs(a.x-b.x) + std::abs(a.y-b.y);
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
    std::uint32_t const N = 300;

    std::vector<point> points;
    auto into_points = std::back_inserter(points);
    ranges::generate_n(into_points, N, cardinal::uniform_walk(rng, origin));

    REQUIRE(points.size() == N);
}

TEST_CASE("adjacency: two", "[single-file]")
{
    std::mt19937 rng(1);
    point origin(0, 0);

    std::vector<point> points;
    auto into_points = std::back_inserter(points);
    ranges::generate_n(into_points, 2, cardinal::uniform_walk(rng, origin));

    REQUIRE(points.size() == 2);
    REQUIRE(distance(points[0], points[1]) == 1);
}

TEST_CASE("adjacency: many", "[single-file]")
{
    std::mt19937 rng(2);
    point origin(0, 0);

    std::vector<point> points;
    std::uint32_t const N = 200;
    points.reserve(N);

    auto into_points = std::back_inserter(points);
    ranges::generate_n(into_points, N, cardinal::uniform_walk(rng, origin));
    REQUIRE(points.size() == N);

    auto adjacent_pair = [](std::uint32_t i) {
        return std::make_pair(i, i+1);
    };
    auto pairs = views::iota(0u, N-1) | views::transform(adjacent_pair);

    auto is_adjacent = [&points](auto const & pair) {
        auto const [i, j] = pair;
        return distance(points[i], points[j]) == 1;
    };
    REQUIRE(ranges::all_of(pairs, is_adjacent));
}

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