#ifndef BYTEME_GZIP_FILE_WRITER_HPP
#define BYTEME_GZIP_FILE_WRITER_HPP

#include <stdexcept>
#include <vector>
#include <optional>

#include "Writer.hpp"
#include "check_buffer_size.hpp"

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
     * Size of the internal buffer used by Zlib. 
     * Larger values usually reduce computational time at the cost of increased memory usage.
     * If no value is supplied, the default buffer size for `gzbuffer()` is not changed.
     */
    std::optional<unsigned> gzbuffer_size;
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
        set_optional_gzbuffer_size(my_gzfile, options.gzbuffer_size);
        if (gzsetparams(my_gzfile.handle, options.compression_level, Z_DEFAULT_STRATEGY) != Z_OK) {
            throw std::runtime_error("failed to set the Gzip compression parameters");
        }
    }

public:
    using Writer::write;

    void write(const unsigned char* buffer, std::size_t n) {
        safe_write<unsigned, true>(
            buffer,
            n,
            [&](const unsigned char* buffer0, unsigned n0) -> void { 
                auto ok = gzwrite(my_gzfile.handle, buffer0, n0);
                if (ok <= 0) {
                    throw std::runtime_error("failed to write to the Gzip-compressed file");
                }
            }
        );
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
