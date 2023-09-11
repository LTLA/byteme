cmake_minimum_required(VERSION 3.24)

project(byteme
    VERSION 1.1.0
    DESCRIPTION "No-frills byte streaming from file"
    LANGUAGES CXX)

add_library(byteme INTERFACE)
add_library(ltla::byteme ALIAS byteme)

include(GNUInstallDirs)
target_include_directories(byteme INTERFACE 
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/ltla_byteme>")

option(BYTEME_FIND_ZLIB "Try to find and link to Zlib for byteme." ON)
if(BYTEME_FIND_ZLIB)
    find_package(ZLIB)
    if (ZLIB_FOUND)
        target_link_libraries(byteme INTERFACE ZLIB::ZLIB)
    endif()
endif()

# Building the test-related machinery, if we are compiling this library directly.
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    option(BYTEME_TESTS "Build byteme's test suite." ON)
else()
    option(BYTEME_TESTS "Build byteme's test suite." OFF)
endif()

if(BYTEME_TESTS)
    include(CTest)
    if(BUILD_TESTING)
        add_subdirectory(tests)
    endif()
endif()

# Installing for find_package.
include(CMakePackageConfigHelpers)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ltla_byteme)

install(TARGETS byteme
    EXPORT bytemeTargets)

install(EXPORT bytemeTargets
    FILE ltla_bytemeTargets.cmake
    NAMESPACE ltla::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ltla_byteme)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/ltla_bytemeConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ltla_byteme)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/ltla_bytemeConfig.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ltla_byteme)
