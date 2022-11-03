#ifndef BYTEME_RAW_BUFFER_READER_HPP
#define BYTEME_RAW_BUFFER_READER_HPP

#include "Writer.hpp"
#include <vector>

/**
 * @file RawBufferWriter.hpp
 *
 * @brief Write bytes to a raw buffer without any extra transformations.
 */

namespace byteme {

/**
 * @brief Write bytes to a raw buffer.
 *
 * This class will append bytes to an internal instance of a `std::vector` without any further transformations.
 * Not much else to say here.
 */
class RawBufferWriter : public Writer {
public:
    /**
     * @param reserved Number of bytes to reserve in the vector.
     */
    RawBufferWriter(size_t reserved = 0) {
        contents.reserve(reserved);
    }

    void write(const unsigned char* buffer, size_t n) {
        contents.insert(contents.end(), buffer, buffer + n);
    }

    void finish() {}

    /**
     * Contents of the buffer so far.
     */
    std::vector<unsigned char> contents;
};

}

#endif
