cmake_minimum_required(VERSION 3.18)
project(basic-example LANGUAGES CXX)
find_package(simulacrum REQUIRED)

file(GLOB sources *.cpp)
add_executable(march ${sources})
target_link_libraries(march PUBLIC sim::simulacrum)
set_target_properties(march PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED true)   