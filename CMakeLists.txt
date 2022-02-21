cmake_minimum_required(VERSION 3.18)

project(random-walk VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# add test sub-directory if specified
option(run_tests "Run tests" off)
if (run_tests)
    add_subdirectory(test)
endif()

# --- create library ---

# specify file interface
include(GNUInstallDirs)
set(random-walk_SRCEXT hpp)

add_library(random-walk INTERFACE)
add_library(pcg::random-walk ALIAS random-walk)

find_package(spatula REQUIRED)
target_link_libraries(random-walk INTERFACE sp::geometry)

target_include_directories(random-walk INTERFACE
    $<BUILD_INTERFACE:${random-walk_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# -- export library --

# install the target and create the export set
install(TARGETS random-walk
        EXPORT random-walkTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

# --- create package for library ---

# include helper functions for creating config files
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/random-walkConfigVersion.cmake
    VERSION ${random-walk_VERSION}
    COMPATIBILITY AnyNewerVersion)

# generate the package configuration file
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    random-walkConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/random-walk)

# allow exporting from build tree
export(EXPORT random-walkTargets
       FILE ${CMAKE_CURRENT_BINARY_DIR}/random-walkTargets.cmake
       NAMESPACE pcg::)

# install the exported targets
install(EXPORT random-walkTargets
        FILE random-walkTargets.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/random-walk
        NAMESPACE pcg::)

# install the generated files
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/random-walkConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/random-walkConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/random-walk)

# install the include files
install(DIRECTORY src/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.${random-walk_SRCEXT}")

export(PACKAGE random-walk)
