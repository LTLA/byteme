#ifndef BYTEME_TEMP_FILE_PATH_HPP
#define BYTEME_TEMP_FILE_PATH_HPP

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
#include <cstdint>
#include <fstream>

/**
 * @file temp_file_path.hpp
 *
 * @brief Define a temporary file path, usually for testing.
 */

namespace byteme {

/**
 * @param prefix Prefix of the basename of the file.
 * @param ext File extension.
 *
 * @return Path to a new file in an appropriate temporary directory with a unique name.
 * The file itself is created at that location, though this may not be thread-safe.
 *
 * This function is wholly intended for unit testing in **byteme** and downstream libraries.
 * Production code should use OS-specific thread-safe alternatives such as `mkstemp()`. 
 */
inline std::string temp_file_path(const std::string& prefix, const std::string& ext) {
    auto path = fs::temp_directory_path();
    path.append(prefix);
    
    // Hopefully, we create a new seed when the function re-runs.
    std::uint64_t seed;
    try {
        std::random_device rd;
        seed = rd();
    } catch (...) {
        auto now = std::chrono::system_clock::now();
        seed = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    }

    std::mt19937_64 rng(seed);
    while (1) {
        auto copy = path;
        copy += std::to_string(rng());
        copy += ext;

        if (!fs::exists(copy)) {
            path = std::move(copy);
            break;
        }
    }

    // Force the creation of the file. This is not entirely thread-safe
    // as the existence check is done separately, but there's no way to
    // guarantee safety without OS-level mechanics.
    std::ofstream dummy(path, std::ofstream::out);
    return path.string();
}

/**
 * @param prefix Prefix of the basename of the file.
 *
 * @return Path to a new file without any extension, see `temp_file_path()` for more details.
 */
inline std::string temp_file_path(const std::string& prefix) {
    return temp_file_path(prefix, "");
}

}

#endif
