find_package(Catch2 REQUIRED)

file(GLOB test_sources *.cpp)
add_executable(test ${test_sources})
target_link_libraries(test Catch2::Catch2 Catch2::Catch2WithMain)
include_directories(../src)