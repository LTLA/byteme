#ifndef BYTEME_WRITER_HPP
#define BYTEME_WRITER_HPP

#include <string>
#include <cstring>
#include <cstddef>

#include "utils.hpp"

/**
 * @file Writer.hpp
 *
 * @brief Write to an output sink.
 */

namespace byteme {

/**
 * @brief Virtual class for writing bytes to a sink.
 */
class Writer {
public:
    /**
     * The destructor should have the same effect as `finish()` if the latter was not previously called.
     */
    virtual ~Writer() = default;

    /**
     * Write the next chunk of bytes to the output sink.
     * This may or may not be internally buffered at the discretion of the concrete class.
     *
     * @param[in] buffer Pointer to the start of an array containing the bytes to be written.
     * @param n Length of the array in `buffer`.
     * This may be zero.
     */
    virtual void write(const unsigned char* buffer, std::size_t n) = 0;

    /**
     * Indicate that the writing has finished and that the relevant clean-up (buffer flushing, file closing, etc.) should be performed.
     * This should be called exactly once, after which no further calls to `write()` or `finish()` should be performed.
     */
    virtual void finish() = 0;
};

}

#endif
