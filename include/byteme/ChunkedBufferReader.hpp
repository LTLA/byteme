#ifndef BYTEME_CHUNKED_BUFFER_READER_HPP
#define BYTEME_CHUNKED_BUFFER_READER_HPP

#include <cstddef>
#include <algorithm>

#include "Reader.hpp"

/**
 * @file ChunkedBufferReader.hpp
 *
 * @brief Read chunks of bytes from a raw buffer.
 */

namespace byteme {

/**
 * @brief Read chunks of bytes from a raw buffer.
 *
 * This is basically the same as `RawBufferReader` except that chunks of bytes are returned on every `load()` call.
 * It is primarily intended for use in tests of **byteme** callers, to ensure that downstream algorithms behave correctly with respect to chunked reads.
 */
class ChunkedBufferReader final : public byteme::Reader {
public:
    /**
     * @param[in] buffer Pointer to an array of bytes, usually containing text.
     * @param length Length of the buffer.
     * @param chunk_size Size of each chunk in bytes.
     */
    ChunkedBufferReader(const unsigned char* buffer, std::size_t length, std::size_t chunk_size) : my_buffer(buffer), my_length(length), my_chunk_size(chunk_size) {
        my_position = -my_chunk_size; // deliberate underflow so that load() overflows to zero.
    }

public:
    bool load() {
        my_position += my_chunk_size; 
        return (my_position < my_length);
    }

    const unsigned char * buffer () const {
        return my_buffer + my_position;
    }

    std::size_t available() const {
        return std::min(my_chunk_size, my_length - my_position);
    }

private:
    const unsigned char* my_buffer;
    std::size_t my_length;
    std::size_t my_position;
    std::size_t my_chunk_size;
};

}

#endif
