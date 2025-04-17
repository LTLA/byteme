#ifndef BYTEME_GZIP_FILE_READER_HPP
#define BYTEME_GZIP_FILE_READER_HPP

#include <cstddef>
#include <stdexcept>
#include <vector>
#include <optional>

#include "zlib.h"

#include "check_buffer_size.hpp"
#include "SelfClosingGzFile.hpp"
#include "Reader.hpp"

/**
 * @file GzipFileReader.hpp
 *
 * @brief Read a Gzip-compressed file.
 */

namespace byteme {

/**
 * @brief Options for `GzipFileReader` construction.
 */
struct GzipFileReaderOptions {
    /**
     * Size of the buffer in which to store the decompressed data. 
     * Larger values usually reduce computational time at the cost of increased memory usage.
     */
    std::size_t buffer_size = 65536;

    /**
     * Size of the internal buffer used by Zlib.
     * Larger values usually reduce computational time at the cost of increased memory usage.
     * If no value is supplied, the default buffer size for `gzbuffer()` is not changed.
     */
    std::optional<unsigned> gzbuffer_size;
};

/**
 * @brief Read uncompressed bytes from a Gzip-compressed file.
 *
 * This is basically a wrapper around Zlib's `gzFile` with correct closing and error checking.
 */
class GzipFileReader final : public Reader {
public:
    /**
     * @param path Path to the file.
     * @param options Further options.
     */
    GzipFileReader(const char* path, const GzipFileReaderOptions& options) : 
        my_gzfile(path, "rb"),
        my_buffer(
            check_buffer_size<int>( // constrained for the gzread return type.
                check_buffer_size<unsigned>( // constrained for the gzread argument type.
                    check_buffer_size(options.buffer_size)
                )
            )
        )
    {
        set_optional_gzbuffer_size(my_gzfile, options.gzbuffer_size);
    }

public:
    bool load() {
        if (my_finished) {
            // We need to check the status explicitly because gzread()
            // doesn't return an error for a truncated file.
            check_status();
            return false;
        }

        const std::size_t bufsize = my_buffer.size();
        auto ret = gzread(my_gzfile.handle, my_buffer.data(), bufsize);
        if (ret < 0) {
            check_status();
            return false;
        }

        my_read = ret;
        my_finished = (my_read < bufsize);
        return true;
    }

    const unsigned char* buffer() const {
        return my_buffer.data();
    }

    std::size_t available() const {
        return my_read;
    }

private:
    SelfClosingGzFile my_gzfile;
    std::vector<unsigned char> my_buffer;
    std::size_t my_read = 0;
    bool my_finished = false;

    void check_status() {
        int status;
        auto msg = gzerror(my_gzfile.handle, &status);
        if (status != Z_OK) {
            throw std::runtime_error(msg);
        }
    }

};

}

#endif
