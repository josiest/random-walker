cmake_minimum_required(VERSION 3.18)

project(basic-walk)

find_package(simulacrum REQUIRED)
find_package(glm REQUIRED)
file(GLOB sources *.cpp)

add_executable(walk ${sources})
target_link_libraries(walk PUBLIC glm sim::simulacrum)
set_target_properties(walk PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED true)
