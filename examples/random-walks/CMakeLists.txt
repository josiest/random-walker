cmake_minimum_required(VERSION 3.18)
project(random-walks LANGUAGES CXX)
# find_package(simulacrum REQUIRED)
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(basic-walk basic-walk/walk.cpp)
add_executable(custom-engine-walk custom-engine/walk.cpp)

add_executable(sfml-walk sfml-example/walk.cpp)
target_link_libraries(sfml-walk PRIVATE sfml-graphics sfml-window sfml-system)

set_target_properties(basic-walk custom-engine-walk sfml-walk PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED true)
include_directories(../include)

# target_compile_options(walk PUBLIC -fconcepts-dgagnostics-depth=2)