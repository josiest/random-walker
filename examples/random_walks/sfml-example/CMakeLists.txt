cmake_minimum_required(VERSION 3.18)
project(sfml-example)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

find_package(RandomWalk REQUIRED)
find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

add_executable(walk walk.cpp)
target_link_libraries(walk pcg::RandomWalk sfml-system sfml-window sfml-graphics)