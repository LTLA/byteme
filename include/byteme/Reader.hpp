#ifndef BYTEME_READER_HPP
#define BYTEME_READER_HPP

#include <cstddef>

/**
 * @file Reader.hpp
 *
 * @brief Read an input source.
 */

namespace byteme {

/**
 * @brief Virtual class for reading bytes from a source.
 */
class Reader {
public:
    virtual ~Reader() = default;

    /**
     * Read the specified number of bytes from the source into a user-supplied buffer.
     *
     * @param[out] buffer Pointer to an array of at least length `n`.
     * @param n Maximum number of bytes to extract from the source. 
     * This may be zero.
     * 
     * @return Number of bytes that were actually extracted from the source.
     * This will be less than or equal to `n`.
     * If less than `n`, the stream of bytes from the source is finished and no more calls to `read()` should be performed.
     */
    virtual std::size_t read(unsigned char* buffer, std::size_t n) = 0;
};

}

#endif
