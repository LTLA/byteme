include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(CMAKE_CXX_STANDARD 17) # for filesystem headers.

enable_testing()

add_executable(
    libtest 
    src/parse_text_file.cpp
    src/parse_gzip_file.cpp
    src/parse_zlib_buffer.cpp
)

target_link_libraries(
    libtest
    gtest_main
    buffin 
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

add_test(NAME buffin_tests COMMAND libtest)
