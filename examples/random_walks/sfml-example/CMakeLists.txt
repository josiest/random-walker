cmake_minimum_required(VERSION 3.18)
project(sfml-example)

find_package(simulacrum REQUIRED)
find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

add_executable(walk walk.cpp)
target_link_libraries(walk PUBLIC
    sfml-system sfml-window sfml-graphics
    sim::simulacrum)
set_target_properties(walk PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED true)