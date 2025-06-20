#ifndef BYTEME_SOME_BUFFER_READER_HPP
#define BYTEME_SOME_BUFFER_READER_HPP

#include <cstddef>
#include <memory>
#include <cstdio>

#include "Reader.hpp"
#include "RawBufferReader.hpp"
#include "ZlibBufferReader.hpp"
#include "magic_numbers.hpp"

/**
 * @file SomeBufferReader.hpp
 *
 * @brief Read a possibly-Gzipped or Zlibbed buffer.
 */

namespace byteme {

/**
 * @brief Options for the `SomeBufferReader` constructor.
 */
struct SomeBufferReaderOptions {
    /**
     * Size of the buffer to use when reading from disk.
     * Larger values usually reduce computational time at the cost of increased memory usage.
     */
    std::size_t buffer_size = cap<std::size_t>(65536);
};

/**
 * @brief Read a buffer that may or may not be Gzip/Zlib-compressed.
 *
 * This class will automatically detect whether `buffer` refers to a text or Gzip/Zlib-compressed buffer, based on the initial magic numbers.
 * After that, it will dispatch appropriately to `RawBufferReader` or `ZlibBufferReader` respectively.
 */
class SomeBufferReader final : public Reader {
public:
    /**
     * @param[in] buffer Pointer to an array containing the possibly compressed data.
     * @param length Length of the `buffer` array.
     * @param options Further options.
     */
    SomeBufferReader(const unsigned char* buffer, std::size_t length, const SomeBufferReaderOptions& options) {
        if (is_zlib(buffer, length) || is_gzip(buffer, length)) {
            ZlibBufferReaderOptions zopt;
            zopt.buffer_size = options.buffer_size;
            my_source.reset(new ZlibBufferReader(buffer, length, zopt));
        } else {
            my_source.reset(new RawBufferReader(buffer, length));
        }
    }

public:
    bool load() {
        return my_source->load();
    }

    const unsigned char* buffer() const {
        return my_source->buffer();
    }

    std::size_t available() const {
        return my_source->available();
    }

private:
    std::unique_ptr<Reader> my_source;
};

}

#endif
