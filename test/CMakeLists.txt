find_package(Catch2 REQUIRED)
find_package(spatula REQUIRED)

file(GLOB test_sources *.cpp)
add_executable(test ${test_sources})

set_target_properties(test PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED true)

target_link_libraries(test PUBLIC
    sp::spatula
    Catch2::Catch2
    Catch2::Catch2WithMain)

target_include_directories(test PUBLIC ../src)