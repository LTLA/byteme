#ifndef BYTEME_GZIP_FILE_READER_HPP
#define BYTEME_GZIP_FILE_READER_HPP

#include <cstddef>
#include <stdexcept>
#include <vector>
#include <optional>

#include "zlib.h"

#include "SelfClosingGzFile.hpp"
#include "Reader.hpp"
#include "utils.hpp"

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
     * Size of the internal buffer used by Zlib.
     * Larger values usually reduce computational time at the cost of increased memory usage.
     * If no value is supplied, the default buffer size for `gzbuffer()` is not changed.
     */
    std::optional<unsigned> buffer_size;
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
    GzipFileReader(const char* path, const GzipFileReaderOptions& options) : my_gzfile(path, "rb") {
        set_optional_gzbuffer_size(my_gzfile, options.buffer_size);
    }

public:
    std::size_t read(unsigned char* buffer, std::size_t n) {
        // While gzbuffer and gzread accepts 'unsigned', the output number of bytes is actually an 'int'!
        // Moreover, if the requested 'len' can't fit in an 'int', gzread will throw an error -
        // see comments on the return value for gzread at https://www.zlib.net/manual.html.
        // So, we forcibly cap the length to 'int'. 
        return safe_read<int>(buffer, n, [this](unsigned char* buffer, int n) -> int {
            const auto ret = gzread(this->my_gzfile.handle, buffer, n);
            if (ret < n) {
                int status;
                const auto msg = gzerror(this->my_gzfile.handle, &status);
                if (status != Z_OK) {
                    throw std::runtime_error(msg);
                }
            }
            return ret;
        });
    }

private:
    SelfClosingGzFile my_gzfile;
};

}

#endif
