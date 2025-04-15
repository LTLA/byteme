#ifndef BYTEME_GZIP_FILE_READER_HPP
#define BYTEME_GZIP_FILE_READER_HPP

#include <cstddef>
#include <stdexcept>
#include <vector>

#include "zlib.h"

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
     * Size of the buffer for Zlib decompression.
     * Larger values usually reduce computational time at the cost of increased memory usage.
     */
    std::size_t buffer_size = 65536;
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
    GzipFileReader(const char* path, const GzipFileReaderOptions& options) : my_gzfile(path, "rb"), my_buffer(options.buffer_size) {}

public:
    bool load() {
        my_read = gzread(my_gzfile.handle, my_buffer.data(), my_buffer.size());
        if (my_read) {
            return true;
        }

        if (!gzeof(my_gzfile.handle)) { 
            int dummy;
            throw std::runtime_error(gzerror(my_gzfile.handle, &dummy));
        }

        return false;
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
};

}

#endif
