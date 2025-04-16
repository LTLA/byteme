#ifndef BYTEME_WRITER_HPP
#define BYTEME_WRITER_HPP

#include <string>
#include <cstring>
#include <cstddef>

#include "check_buffer_size.hpp"

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
    virtual ~Writer() = default;

    /**
     * Write the next chunk of bytes to the output sink.
     * This may or may not be internally buffered at the discretion of the concrete class.
     *
     * @param[in] buffer Pointer to the start of an array containing the bytes to be written.
     * @param n Length of the array in `buffer`.
     */
    virtual void write(const unsigned char* buffer, std::size_t n) = 0;

    /**
     * Indicate that the writing has finished and that the relevant clean-up (buffer flushing, file closing, etc.) should be performed.
     * This should be called exactly once, after which no further calls to `write()` or `finish()` should be performed.
     */
    virtual void finish() = 0;

    /**
     * Write a string to the output sink.
     *
     * @param x String to be written.
     */
    void write(const std::string& x) {
        // Protect against cases where size_t < string::size_type.
        safe_write<std::size_t, false>(
            reinterpret_cast<const unsigned char*>(x.c_str()),
            x.size(),
            [&](const unsigned char* ptr0, std::size_t n0) -> void {
                write(ptr0, n0);
            }
        );
    }

    /**
     * Write a null-terminated C-string to the output sink.
     *
     * @param[in] x Pointer to the string to be written.
     */
    void write(const char* x) {
        write(reinterpret_cast<const unsigned char*>(x), std::strlen(x));
    }

    /**
     * Write a single character to the output sink.
     *
     * @param x Character to be written.
     */
    void write(char x) {
        write(reinterpret_cast<const unsigned char*>(&x), 1);
    }
};

}

#endif
