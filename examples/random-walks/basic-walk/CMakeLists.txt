cmake_minimum_required(VERSION 3.18)
project(basic-walk LANGUAGES CXX)
find_package(simulacrum REQUIRED)

add_executable(walk walk.cpp)

target_link_libraries(walk PUBLIC sim::simulacrum)
set_target_properties(walk PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED true)   

target_compile_options(walk PUBLIC -fconcepts-diagnostics-depth=2)
