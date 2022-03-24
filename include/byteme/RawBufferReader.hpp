#ifndef BUFFIN_RAW_BUFFER_READER_HPP
#define BUFFIN_RAW_BUFFER_READER_HPP

#include <algorithm>

/**
 * @file RawBufferReader.hpp
 *
 * @brief Read bytes from a raw buffer without any extra transformations.
 */

namespace byteme {

/**
 * @brief Read bytes from a raw buffer, usually text.
 *
 * This is a wrapper around an input buffer, provided for consistency with the other `*Reader` classes.
 * We assume that the lifetime of the data in the `buffer` pointer exceeds the lifetime of the instance.
 *
 * @tparam B Buffer type, usually a `char` or `unsigned char`.
 */
template<typename B = char>
class RawBufferReader {
public:
    /**
     * @param buffer Buffer containing text.
     * @param buffer_size Size of the buffer to use for reading.
     */
    RawBufferReader(const B* buffer, size_t length) : buffer_(buffer), len_(length) {}

    /**
     * Read the next chunk of bytes.
     * In practice, this doesn't do anything because all bytes are already available.
     *
     * @return `false`, as all bytes are now available.
     */
    bool operator()() {
        return false;
    }

    /**
     * @return Pointer to the start of an array containing the bytes.
     * The number of available bytes is provided in `available()`.
     */
    const B* buffer() const {
        return buffer_;
    }

    /**
     * @return Number of decompressed bytes available in the `buffer()`.
     */
    size_t available() const {
        return len_;
    }

private:
    const B* buffer_;
    size_t len_;
};

}

#endif
