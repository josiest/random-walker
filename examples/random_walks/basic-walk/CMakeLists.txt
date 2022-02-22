cmake_minimum_required(VERSION 3.18)

project(basic-walk LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

find_package(simulacrum REQUIRED)
file(GLOB sources *.cpp)

add_executable(walk ${sources})
target_link_libraries(walk INTERFACE sim::simulacrum)

# Necessary for current windows cmake bug
if (WIN32)
    find_package(spatula REQUIRED)
    get_target_property(simulacrum_includes sim::simulacrum INTERFACE_INCLUDE_DIRECTORIES)
    get_target_property(spatula_includes sp::spatula INTERFACE_INCLUDE_DIRECTORIES)
    target_include_directories(walk PRIVATE ${simulacrum_includes} ${spatula_includes})
endif()