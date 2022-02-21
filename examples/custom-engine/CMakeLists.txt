cmake_minimum_required(VERSION 3.18)

project(basic-walk)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

find_package(RandomWalk REQUIRED)
find_package(glm REQUIRED)
file(GLOB sources *.cpp)

add_executable(walk ${sources})
target_link_libraries(walk PUBLIC pcg::RandomWalk glm)
