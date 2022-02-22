cmake_minimum_required(VERSION 3.18)

project(basic-walk)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

find_package(simulacrum REQUIRED)
find_package(glm REQUIRED)
file(GLOB sources *.cpp)

add_executable(walk ${sources})
target_link_libraries(walk PRIVATE glm)
target_link_libraries(walk INTERFACE sim::simulacrum)
if (WIN32)
    find_package(spatula REQUIRED)
    get_target_property(sim_includes sim::simulacrum INTERFACE_INCLUDE_DIRECTORIES)
    get_target_property(sp_includes sp::spatula INTERFACE_INCLUDE_DIRECTORIES)
    target_include_directories(walk PRIVATE ${sim_includes} ${sp_includes})
endif()