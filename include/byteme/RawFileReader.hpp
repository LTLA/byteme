#ifndef BYTEME_RAW_FILE_READER_HPP
#define BYTEME_RAW_FILE_READER_HPP

#include <vector>
#include <stdexcept>
#include <cstdio>
#include <cstddef>
#include <optional>

#include "Reader.hpp"
#include "SelfClosingFILE.hpp"

/**
 * @file RawFileReader.hpp
 *
 * @brief Read a file without any extra transformations.
 */

namespace byteme {

/**
 * @brief Options for the `RawFileReader` constructor.
 */
struct RawFileReaderOptions {
    /**
     * Size of the internal buffer used by `setvbuf()`.
     * Larger values usually reduce computational time at the cost of increased memory usage.
     * If no value is supplied, the default buffer size is not changed.
     */
    std::optional<std::size_t> buffer_size;
};

/**
 * @brief Read bytes from a file, usually text.
 *
 * This is basically a simple wrapper around `std::FILE` structures, with correct closing and error checking.
 */
class RawFileReader final : public Reader {
public:
    /**
     * @param path Path to the file.
     * @param options Further options.
     */
    RawFileReader(const char* path, const RawFileReaderOptions& options) : my_file(path, "rb") {
        set_optional_bufsiz(my_file, options.buffer_size);
    }

public:
    std::size_t read(unsigned char* buffer, std::size_t n) {
        auto& handle = my_file.handle;
        auto read = std::fread(buffer, sizeof(unsigned char), n, handle);
        if (read < n && std::ferror(handle)) {
            throw std::runtime_error("file read failed");
        }
        return read;
    }

private:
    SelfClosingFILE my_file;
};

}

#endif
