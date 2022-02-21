#include <catch2/catch_test_macros.hpp>
#include "pcg/random_walk.hpp"

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
using namespace pcg;

template<vector2 Vector>
using walker = cardinal::walker<Vector>;

bool is_adjacent(point const & a, point const & b)
{
    return std::abs(a.x-b.x) + std::abs(a.y-b.y) == 1;
}

struct pointf {
    float x; float y;
    pointf() : x(0), y(0) {}
    pointf(float x, float y) : x(x), y(y) {}
    pointf & operator+=(pointf q)
    {
        x += q.x;
        y += q.y;
        return *this;
    }
};
pointf operator+(pointf const & a, pointf const & b)
{
    return pointf(a.x + b.x, a.y + b.y);
}
bool is_adjacentf(pointf const & p, pointf const & q)
{
    return std::abs(p.x-q.x) + std::abs(p.y-q.y) - 1.f < .01f;
}

namespace std {

ostream & operator<<(ostream & os, point const & p)
{
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}
}

TEST_CASE("walk-size: empty", "[single-file]")
{
    std::mt19937 rng;
    point const origin(0, 0);
    walker homer(origin);

    std::vector<point> points;
    auto into_points = std::back_inserter(points);
    ranges::generate_n(into_points, 0, cardinal::uniform_walk(rng, homer));

    REQUIRE(points.empty());
}

TEST_CASE("walk-size: one", "[single-file]")
{
    std::mt19937 rng;
    point const origin(0, 0);
    walker homer(origin);

    std::vector<point> points;
    auto into_points = std::back_inserter(points);
    ranges::generate_n(into_points, 1, cardinal::uniform_walk(rng, homer));

    REQUIRE(points.size() == 1);
}

TEST_CASE("walk-size: many", "[single-file]")
{
    std::mt19937 rng;
    point const origin(0, 0);
    walker homer(origin);

    std::vector<point> points;
    std::uint32_t const N = 300;
    points.reserve(N);

    auto into_points = std::back_inserter(points);
    ranges::generate_n(into_points, N, cardinal::uniform_walk(rng, homer));

    REQUIRE(points.size() == N);
}

TEST_CASE("adjacency: two", "[single-file]")
{
    std::mt19937 rng(1);
    point const origin(0, 0);
    walker homer(origin);

    std::vector<point> points;
    auto into_points = std::back_inserter(points);
    ranges::generate_n(into_points, 2, cardinal::uniform_walk(rng, homer));

    REQUIRE(points.size() == 2);
    REQUIRE(is_adjacent(points[0], points[1]));
}

TEST_CASE("adjacency: many", "[single-file]")
{
    std::mt19937 rng(2);
    point const origin(0, 0);
    walker homer(origin);

    std::vector<point> points;
    std::uint32_t const N = 200;
    points.reserve(N);

    auto into_points = std::back_inserter(points);
    ranges::generate_n(into_points, N, cardinal::uniform_walk(rng, homer));
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
    pointf const origin{-1.f, -2.f};
    walker homer(origin);

    std::array<pointf, 1> points;
    ranges::generate(points, cardinal::uniform_walk(rng, homer));
    REQUIRE(is_adjacentf(origin, points[0]));
}

TEST_CASE("start: positive components", "[single-file]")
{
    std::mt19937 rng(4);
    point const start(341, 202);
    walker homer(start);

    std::array<point, 1> points;
    ranges::generate(points, cardinal::uniform_walk(rng, homer));

    REQUIRE(is_adjacent(start, points[0]));
}

TEST_CASE("start: negative components" "[single-file]")
{
    std::mt19937 rng(5);
    point const start(943, -1281);
    walker homer(start);

    std::array<point, 1> points;
    ranges::generate(points, cardinal::uniform_walk(rng, homer));

    REQUIRE(is_adjacent(start, points[0]));
}