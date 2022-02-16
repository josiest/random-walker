cmake_minimum_required(VERSION 3.18)

project(random-walker)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

file(GLOB sources src/*.cpp)

add_executable(walk ${sources})
include_directories(include)
