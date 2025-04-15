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
 * We assume that the lifetime of the data in the `buffer` pointer exceeds the lifetime of the instance.
 */
class RawBufferReader final : public Reader {
public:
    /**
     * @param[in] buffer Pointer to an array of bytes. 
     * @param length Length of the buffer.
     */
    RawBufferReader(const unsigned char* buffer, std::size_t length) : my_buffer(buffer), my_length(length) {}

public:
    bool load() {
        if (my_used) {
            return false;
        }
        my_used = true;
        return true;
    }

    const unsigned char* buffer() const {
        return my_buffer;
    }

    std::size_t available() const {
        return my_length;
    }

private:
    const unsigned char* my_buffer;
    std::size_t my_length;
    bool my_used = false;
};

}

#endif
