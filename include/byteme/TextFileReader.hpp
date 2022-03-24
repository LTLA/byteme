#ifndef BYTEME_TEXTFILEREADER_HPP
#define BYTEME_TEXTFILEREADER_HPP

#include <vector>
#include <stdexcept>
#include <string>
#include <cstdio>

/**
 * @file TextFileReader.hpp
 *
 * @brief Read a text file.
 */

namespace buffin {

/**
 * @brief Read bytes from a text file.
 *
 * This is basically a simple wrapper around `FILE` structures, with correct closing and error checking.
 * Mostly provided because I always forget how to interact with `ifstream` objects when I want a sequence of bytes.
 */
class TextFileReader {
private:
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

public:
    TextFileReader(const char* path, size_t buffer_size = 65536) : file(path), buffer_(buffer_size), read(0) {}

    /**
     * Read the next chunk of bytes from the input file.
     *
     * To read the entire file, this function should be called repeatedly until `false` is returned.
     * Note that `buffer()` and `available()` may still be valid on the last invocation (i.e., the one that returns `false`),
     * as some bytes may have been read before reaching the end of the file.
     *
     * @return Boolean indicating whether there are still bytes remaining in the file.
     */
    bool operator() {
        auto& handle = file.handle;
        read = std::fread(buffer_.data(), sizeof(char), buffer_size, handle);
        if (read != buffer_size) {
            if (std::feof(handle)) {
                return false;
            } else {
                throw std::runtime_error("failed to read '" + std::string(path) + "' (error code " + std::to_string(std::ferror(handle)) + ")");
            }
        }
        return true;
    }

    /**
     * @return Pointer to the start of an array containing the decompressed bytes.
     * The number of available bytes is provided in `available()`.
     */
    const unsigned char* buffer() const  {
        return buffer_.data();
    }

    /**
     * @return Number of decompressed bytes available in the `buffer()`.
     */
    size_t available() const {
        return read;
    }

private:
    SelfClosingFILE file;
    std::vector<char> buffer;
    size_t read;
};

}

#endif
