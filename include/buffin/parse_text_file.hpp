#ifndef BUFFIN_PARSE_TEXT_FILE_HPP
#define BUFFIN_PARSE_TEXT_FILE_HPP

#include <vector>
#include <stdexcept>
#include <string>
#include <cstdio>

/**
 * @file parse_text_file.hpp
 *
 * @brief Parse a text file without any compression.
 */

namespace buffin {

/**
 * @cond
 */
struct SelfClosingFILE {
    SelfClosingFILE(const char * path) : handle(std::fopen(path, "rb")) {
        if (!handle) {
            throw std::runtime_error("failed to open file at '" + std::string(path) + "'");
        }
        return;
    }

    ~SelfClosingFILE() {
        std::fclose(handle);
        return;
    }

    // Delete the remaining constructors.
    SelfClosingFILE(const SelfClosingFILE&) = delete;
    SelfClosingFILE(SelfClosingFILE&&) = delete;
    SelfClosingFILE& operator=(const SelfClosingFILE&) = delete;
    SelfClosingFILE& operator=(SelfClosingFILE&&) = delete;

    FILE* handle;
};
/**
 * @endcond
 */

/**
 * @tparam Parser A parser class with an `add()` method.
 *
 * @param path Path to the file.
 * @param parser Instance of the parser class.
 * @param buffer_size Size of the buffer to use for reading.
 *
 * @return The file's contents are iteratively read into a buffer that is passed to `Parser::add()` until completion.
 */
template<class Parser>
void parse_text_file(const char* path, Parser& parser, size_t buffer_size = 65536) {
    SelfClosingFILE file(path);
    auto& handle = file.handle;

    std::vector<char> buffer(buffer_size); 
    while (1) {
        size_t available = std::fread(buffer.data(), sizeof(char), buffer_size, handle);

        if (available != buffer_size) {
            if (std::feof(handle)) {
                parser.add(buffer.data(), available);
                break;
            } else {
                throw std::runtime_error("failed to read '" + std::string(path) + "' (error code " + std::to_string(std::ferror(handle)) + ")");
            }
        } else {
            parser.add(buffer.data(), available);
        }
    }

    return;
}

}

#endif
