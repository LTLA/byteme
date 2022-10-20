include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/afd902e992b720d1b3e106bc5e425a5768872265.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
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
    src/PerByte.cpp
)

target_link_libraries(
    libtest
    gtest_main
    byteme 
)

find_package(ZLIB)
target_link_libraries(libtest ZLIB::ZLIB)

include(CheckIncludeFiles)
check_include_files(filesystem HAVE_CXX_FS)
if (NOT HAVE_CXX_FS) 
    target_link_libraries(libtest stdc++fs) 
endif()

include(GoogleTest)
gtest_discover_tests(libtest)
