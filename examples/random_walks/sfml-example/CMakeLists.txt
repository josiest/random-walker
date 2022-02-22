cmake_minimum_required(VERSION 3.18)
project(sfml-example)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

find_package(simulacrum REQUIRED)
find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

add_executable(walk walk.cpp)
target_link_libraries(walk PRIVATE sfml-system sfml-window sfml-graphics)
target_link_libraries(walk INTERFACE sim::simulacrum)
if (WIN32)
    find_package(spatula REQUIRED)
    get_target_property(simulacrum_includes
        sim::simulacrum
        INTERFACE_INCLUDE_DIRECTORIES)
    get_target_property(spatula_includes
        sp::spatula
        INTERFACE_INCLUDE_DIRECTORIES)
    target_include_directories(walk PRIVATE
        ${simulacrum_includes}
        ${spatula_includes})
endif()