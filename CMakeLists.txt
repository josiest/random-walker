cmake_minimum_required(VERSION 3.18)

project(basic-walk)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

file(GLOB sources include/pcg/*.cpp)

option(testing off)
if (testing)
    add_subdirectory(test)
endif()