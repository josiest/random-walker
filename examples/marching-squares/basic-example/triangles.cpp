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
using vertex_buffer = std::verctor<glm::vec2>;

namespace std {
ostream & operator<<(ostream & os, glm::vec2 const & v)
{
   os << "(" << v.x << ", " << v.y << ")";
   return os;
}
}

void print_triangle(glm::vec2 const & A, glm::vec2 const & B,
                    glm::vec2 const & C)
{
    std::cout << "Triangle:" << std::endl
              << "  " << A << std::endl
              << "  " << B << std::endl
              << "  " << C << std::endl;
}

auto print_cell(unsigned_buffer const & indices,
                unsigned_buffer const & counts,
                vertex_buffer const & verts)
{
    return [&indices, &counts, &verts](std::uint32_t i) {
        std::cout << "=== Cell ===" << std::endl;
        ranges::for_each(ms::cell_triangles(verts, indices[i], counts[i]),
                         print_triangle);
    };
}

int main()
{
    std::array<glm::ivec2, 1> points{ {0, 0} };

    unsigned_buffer indices;
    unsigned_buffer counts;
    vertex_buffer verts;

    auto into_indices = std::back_inserter(indices);
    auto into_counts = std::back_inserter(counts);
    auto into_verts = std::back_inserter(verts);

    ms::triangulate(points, into_indices, into_counts, into_verts);
    ranges::for_each(views::iota(0, points.size())
                     print_cell(indices, counts, verts));
}