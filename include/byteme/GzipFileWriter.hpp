#ifndef BYTEME_GZIP_FILE_WRITER_HPP
#define BYTEME_GZIP_FILE_WRITER_HPP

#include <stdexcept>
#include <vector>
#include <string>
#include "Writer.hpp"

/**
 * @file GzipFileWriter.hpp
 *
 * @brief Write a Gzip-compressed file.
 */

namespace byteme {

/**
 * @brief Options for `GzipFileWriter` construction.
 */
struct GzipFileWriterOptions {
    /**
     * Gzip compression level, from 1 to 9.
     * Larger values improve compression efficiency at the cost of some computational work.
     */
    int compression_level = 6;

    /**
     * Size of the internal buffer to pass to Zlib. 
     * Larger values usually reduce computational time at the cost of increased memory usage.
     */
    size_t buffer_size = 65536;
};

/**
 * @brief Write uncompressed bytes to a Gzip-compressed file.
 *
 * This is basically a wrapper around Zlib's `gzFile` with correct closing and error checking.
 */
class GzipFileWriter final : public Writer {
public:
    /**
     * @param path Path to the file.
     * @param options Further options.
     */
    GzipFileWriter(const char* path, const GzipFileWriterOptions& options) : my_gzfile(path, "wb") {
        if (gzbuffer(my_gzfile.handle, options.buffer_size)) {
            throw std::runtime_error("failed to set the Gzip compression buffer");
        }
        if (gzsetparams(my_gzfile.handle, options.compression_level, Z_DEFAULT_STRATEGY) != Z_OK) {
            throw std::runtime_error("failed to set the Gzip compression parameters");
        }
    }

    /**
     * @param path Path to the file.
     * @param options Further options.
     */
    GzipFileWriter(const std::string& path, const GzipFileWriterOptions& options) : GzipFileWriter(path.c_str(), options) {}

public:
    using Writer::write;

    void write(const unsigned char* buffer, size_t n) {
        if (n) {
            size_t ok = gzwrite(my_gzfile.handle, buffer, n);
            if (ok != n) {
                throw std::runtime_error("failed to write to the Gzip-compressed file");
            }
        }
    }

    void finish() {
        my_gzfile.closed = true;
        if (gzclose(my_gzfile.handle) != Z_OK) {
            throw std::runtime_error("failed to close the Gzip-compressed file after writing");
        }
    }

private:
    SelfClosingGzFile my_gzfile;
};

}

#endif
