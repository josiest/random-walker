#include <catch2/catch_test_macros.hpp>
#include "simulacrum/random_walks.hpp"
#include <spatula/geometry.hpp>

// math and algorithms
#include <random>
#include <cmath>
#include <algorithm>

// data structures and data types
#include <vector>
#include <array>
#include <cstdint>

// i/o
#include <iostream>

namespace ranges = std::ranges;
namespace views = std::views;
using namespace simulacrum;
using cardinal = sp::cardinal::direction_name;

struct point { int x, y; };
struct pointf { float x, y; };

bool is_adjacent(point const & a, point const & b)
{
    return std::abs(a.x-b.x) + std::abs(a.y-b.y) == 1;
}
bool is_adjacentf(pointf const & p, pointf const & q)
{
    return std::abs(p.x-q.x) + std::abs(p.y-q.y) - 1.f < .01f;
}

point operator+(point const & a, point const & b)
{
    return point(a.x + b.x, a.y + b.y);
}
pointf operator+(pointf const & a, pointf const & b)
{
    return pointf(a.x + b.x, a.y + b.y);
}

bool operator==(point const & a, point const & b)
{
    return a.x == b.x and a.y == b.y;
}
bool operator==(pointf const & a, pointf const & b)
{
    return a.x == b.x and a.y == b.y;
}

namespace std {

template<sp::basic_vector2 Vector>
ostream & operator<<(ostream & os, Vector const & p)
{
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}
}

TEST_CASE("walk-size: empty", "[single-file]")
{
    std::mt19937 rng;
    point const origin(0, 0);

    std::vector<point> points;
    auto into_points = std::back_inserter(points);
    ranges::generate_n(into_points, 0, uniform_walk<cardinal>(rng, origin));

    REQUIRE(points.empty());
}

TEST_CASE("walk-size: one", "[single-file]")
{
    std::mt19937 rng;
    point const origin(0, 0);

    std::vector<point> points;
    auto into_points = std::back_inserter(points);
    ranges::generate_n(into_points, 1, uniform_walk<cardinal>(rng, origin));

    REQUIRE(points.size() == 1);
}

TEST_CASE("walk-size: many", "[single-file]")
{
    std::mt19937 rng;
    point const origin(0, 0);

    std::vector<point> points;
    std::uint32_t const N = 300;
    points.reserve(N);

    auto into_points = std::back_inserter(points);
    ranges::generate_n(into_points, N, uniform_walk<cardinal>(rng, origin));

    REQUIRE(points.size() == N);
}

TEST_CASE("adjacency: two", "[single-file]")
{
    std::mt19937 rng(1);
    point const origin(0, 0);

    std::vector<point> points;
    auto into_points = std::back_inserter(points);
    ranges::generate_n(into_points, 2, uniform_walk<cardinal>(rng, origin));

    REQUIRE(points.size() == 2);
    REQUIRE(is_adjacent(points[0], points[1]));
}

TEST_CASE("adjacency: many", "[single-file]")
{
    std::mt19937 rng(2);
    point const origin(0, 0);

    std::vector<point> points;
    std::uint32_t const N = 200;
    points.reserve(N);

    auto into_points = std::back_inserter(points);
    ranges::generate_n(into_points, N, uniform_walk<cardinal>(rng, origin));
    REQUIRE(points.size() == N);

    auto adjacent_pair = [](std::uint32_t i) {
        return std::make_pair(i, i+1);
    };
    auto pairs = views::iota(0u, N-1) | views::transform(adjacent_pair);

    auto pair_is_adjacent = [&points](auto const & pair) {
        auto const [i, j] = pair;
        return is_adjacent(points[i], points[j]);
    };
    REQUIRE(ranges::all_of(pairs, pair_is_adjacent));
}

TEST_CASE("generic: float-point", "[single-file]")
{
    std::mt19937 rng(3);
    pointf const start{-1.f, -2.f};

    std::array<pointf, 1> points;
    ranges::generate(points, uniform_walk<cardinal>(rng, start));
    REQUIRE(is_adjacentf(start, points[0]));
}

TEST_CASE("start: positive components", "[single-file]")
{
    std::mt19937 rng(4);
    point const start(341, 202);

    std::array<point, 2> points;
    ranges::generate(points, uniform_walk<cardinal>(rng, start));

    REQUIRE(is_adjacent(start, points[1]));
}

TEST_CASE("start: negative components" "[single-file]")
{
    std::mt19937 rng(5);
    point const start(943, -1281);

    std::array<point, 2> points;
    ranges::generate(points, uniform_walk<cardinal>(rng, start));

    REQUIRE(is_adjacent(start, points[1]));
}