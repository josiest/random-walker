#include <limits>
#include <cstddef>
#include <ctime>

#include <simulacrum/random_walks.hpp>
#include <glm/glm.hpp>
#include <array>
#include <algorithm>
#include <random>
#include <cstdint>

#include <iostream>

namespace ranges = std::ranges;
namespace cardinal = simulacrum::cardinal;

struct random_engine {
    // I think this is a bug - gcc requires there to be a result_type
    // however - this isn't a requirement in the C++ specification,
    // and it could certainly be deduced fairly easily
    using result_type = std::size_t;

    std::size_t seed;
    std::size_t operator()()
    {
        std::size_t const current_seed = seed;
        seed = seed*0x3243f6a8885a308dU + 1;
        return current_seed;
    }

    static constexpr std::size_t min()
    {
        return std::numeric_limits<std::size_t>::min();
    }

    static constexpr std::size_t max()
    {
        return std::numeric_limits<std::size_t>::max();
    }
};

void print(glm::ivec2 const & p)
{
    std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
}

int main()
{
    random_engine rng{static_cast<std::size_t>(std::time(nullptr))};

    constexpr std::uint32_t N = 10;
    std::array<glm::ivec2, N> points;

    glm::ivec2 const origin(0, 0);
    cardinal::walker homer(origin);

    ranges::generate(points, cardinal::uniform_walk(rng, homer));
    ranges::for_each(points, print);
}