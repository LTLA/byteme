#ifndef BYTEME_RAW_BUFFER_READER_HPP
#define BYTEME_RAW_BUFFER_READER_HPP

#include <algorithm>
#include <cstddef>

#include "Reader.hpp"

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
 */
class RawBufferReader final : public Reader {
public:
    /**
     * @param[in] buffer Pointer to an array of bytes. 
     * We assume that the lifetime of the array exceeds the lifetime of this `RawBufferReader` instance.
     * @param length Length of the buffer.
     */
    RawBufferReader(const unsigned char* buffer, std::size_t length) : my_buffer(buffer), my_remaining(length) {}

public:
    std::size_t read(unsigned char* buffer, std::size_t n) {
        const auto to_copy = std::min(n, my_remaining);
        std::copy_n(my_buffer, to_copy, buffer);
        my_buffer += to_copy;
        my_remaining -= to_copy;
        return to_copy;
    }

private:
    const unsigned char* my_buffer;
    std::size_t my_remaining;
};

}

#endif
