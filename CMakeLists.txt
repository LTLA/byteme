cmake_minimum_required(VERSION 3.14)

project(byteme
    VERSION 1.0.0
    DESCRIPTION "No-frills byte streaming from file"
    LANGUAGES CXX)

add_library(byteme INTERFACE)

target_include_directories(byteme INTERFACE include/)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
    if(BUILD_TESTING)
        add_subdirectory(tests)
    endif()
endif()
