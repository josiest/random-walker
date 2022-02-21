cmake_minimum_required(VERSION 3.18)

project(RandomWalk VERSION 1.0.0)

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
set(RandomWalk_SRCEXT hpp)

add_library(RandomWalk INTERFACE)
add_library(pcg::RandomWalk ALIAS RandomWalk)

target_include_directories(RandomWalk INTERFACE
    $<BUILD_INTERFACE:${RandomWalk_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# -- export library --

# install the target and create the export set
install(TARGETS RandomWalk
        EXPORT RandomWalkTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

# --- create package for library ---

# include helper functions for creating config files
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/RandomWalkConfigVersion.cmake
    VERSION ${RandomWalk_VERSION}
    COMPATIBILITY AnyNewerVersion)

# generate the package configuration file
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    RandomWalkConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/RandomWalk)

# allow exporting from build tree
export(EXPORT RandomWalkTargets
       FILE ${CMAKE_CURRENT_BINARY_DIR}/RandomWalkTargets.cmake
       NAMESPACE pcg::)

# install the exported targets
install(EXPORT RandomWalkTargets
        FILE RandomWalkTargets.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/RandomWalk
        NAMESPACE pcg::)

# install the generated files
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/RandomWalkConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/RandomWalkConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/RandomWalk)

# install the include files
install(DIRECTORY src/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.${RandomWalk_SRCEXT}")

export(PACKAGE RandomWalk)
