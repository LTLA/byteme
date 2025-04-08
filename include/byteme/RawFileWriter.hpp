#ifndef BYTEME_RAW_FILE_WRITER_HPP
#define BYTEME_RAW_FILE_WRITER_HPP

#include <vector>
#include <stdexcept>
#include <cstdio>
#include "Writer.hpp"
#include "SelfClosingFILE.hpp"

/**
 * @file RawFileWriter.hpp
 *
 * @brief Write bytes to a file without any extra transformations.
 */

namespace byteme {

/**
 * @brief Options for the `RawFileWriter` constructor.
 */
struct RawFileWriterOptions {
    /**
     * Size of the buffer to use when writing to disk.
     * Larger values usually reduce computational time at the cost of increased memory usage.
     */
    size_t buffer_size = 65536;
};

/**
 * @brief Write bytes to a file.
 *
 * This class will write bytes to a file without any further transformations.
 * It is basically a simple wrapper around `FILE` structures, with correct closing and error checking.
 */
class RawFileWriter final : public Writer {
public:
    /**
     * @param path Path to the file.
     * @param options Further options.
     */
    RawFileWriter(const char* path, const RawFileWriterOptions& options) : my_file(path, "wb") {
        if (std::setvbuf(my_file.handle, nullptr, _IOFBF, options.buffer_size)) {
            throw std::runtime_error("failed to set a buffer size for file writing");
        }
    }

public:
    using Writer::write;

    void write(const unsigned char* buffer, size_t n) {
        size_t ok = std::fwrite(buffer, sizeof(unsigned char), n, my_file.handle);
        if (ok < n) {
            throw std::runtime_error("failed to write raw binary file (fwrite error " + std::to_string(std::ferror(my_file.handle)) + ")");
        }
    }

    void finish() {
        if (std::fclose(my_file.handle)) {
            throw std::runtime_error("failed to close raw binary file");
        }
        my_file.handle = nullptr;
    }

private:
    SelfClosingFILE my_file;
};

}

#endif
