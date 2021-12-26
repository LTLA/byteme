#ifndef TEMP_FILE_PATH_H
#define TEMP_FILE_PATH_H

#if __has_include(<filesystem>)
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif

#include <random>
#include <chrono>
#include <string>

inline std::string temp_file_path(std::string base) {
    auto path = fs::temp_directory_path();
    path.append(base);
    
    auto now = std::chrono::system_clock::now();
    auto seed = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();

    // Not thread safe, but it should be good enough for testing.
    std::mt19937_64 rng(seed);
    while (1) {
        auto path2 = path;
        path2 += std::to_string(rng());
        if (!fs::exists(path2)) {
            return path2;
        }
    }

    return path;
}

#endif
