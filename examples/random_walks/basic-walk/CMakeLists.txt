cmake_minimum_required(VERSION 3.18)

project(basic-walk LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

find_package(simulacrum REQUIRED)
file(GLOB sources *.cpp)

add_executable(walk ${sources})
target_link_libraries(walk sim::simulacrum)