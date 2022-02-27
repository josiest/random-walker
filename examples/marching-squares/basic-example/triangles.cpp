#include <simulacrum/marching_squares.hpp>

// data strutures and algorithms
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>

// i/o
#include <iostream>

// aliases
namespace ms = simulacrum::marching_squares;
namespace ranges = std::ranges;
using unsigned_buffer = std::vector<std::uint32_t>;
using vertex_buffer = std::vector<glm::vec2>;

namespace std {
ostream & operator<<(ostream & os, glm::vec2 const & v)
{
   os << "(" << v.x << ", " << v.y << ")";
   return os;
}
}
void print(glm::vec2 const & v) { std::cout << v << "\n"; }

int main()
{
    std::array<glm::ivec2, 1> points{ glm::ivec2{0, 0} };

    unsigned_buffer indices;
    unsigned_buffer counts;
    vertex_buffer verts;

    auto into_indices = std::back_inserter(indices);
    auto into_counts = std::back_inserter(counts);
    auto into_verts = std::back_inserter(verts);

    ms::triangulate<glm::vec2>(points, into_indices, into_counts, into_verts);
    ranges::for_each(verts, print);
}