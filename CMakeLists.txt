cmake_minimum_required(VERSION 3.14)

project(aarand
    VERSION 1.0.0
    DESCRIPTION "C++ library to parse buffered inputs"
    LANGUAGES CXX)

add_library(buffin INTERFACE)

target_include_directories(buffin INTERFACE include/)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
    if(BUILD_TESTING)
        add_subdirectory(tests)
    endif()
endif()
