include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/afd902e992b720d1b3e106bc5e425a5768872265.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Avoid installing GoogleTest when installing this project.
option(INSTALL_GTEST "Enable installation of googletest." OFF)

FetchContent_MakeAvailable(googletest)

set(CMAKE_CXX_STANDARD 17) # for filesystem headers.

enable_testing()

add_executable(
    libtest 

    src/GzipFileReader.cpp
    src/IstreamReader.cpp
    src/RawBufferReader.cpp
    src/RawFileReader.cpp
    src/ZlibBufferReader.cpp
    src/ChunkedBufferReader.cpp
    src/PerByte.cpp

    src/RawBufferWriter.cpp
    src/RawFileWriter.cpp
    src/GzipFileWriter.cpp
    src/ZlibBufferWriter.cpp
    src/OstreamWriter.cpp

    src/byteme.cpp
)

target_link_libraries(
    libtest
    gtest_main
    gmock_main
    byteme 
)

target_compile_options(libtest PRIVATE -Wall -Wextra -Wpedantic -Werror)

include(CheckIncludeFiles)
check_include_files(filesystem HAVE_CXX_FS)
if (NOT HAVE_CXX_FS) 
    target_link_libraries(libtest stdc++fs) 
endif()

set(CODE_COVERAGE OFF CACHE BOOL "Enable coverage testing")
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(libtest PRIVATE -O0 -g --coverage)
    target_link_options(libtest PRIVATE --coverage)
endif()

include(GoogleTest)
gtest_discover_tests(libtest)
