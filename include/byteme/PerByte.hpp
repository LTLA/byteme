#ifndef BYTEME_PERBYTE_HPP
#define BYTEME_PERBYTE_HPP

/**
 * @file PerByte.hpp
 * @brief Perform byte-by-byte extraction.
 */

#include "Reader.hpp"

namespace byteme {

/**
 * @brief Perform byte-by-byte extraction from a `Reader` source.
 *
 * @tparam T Character type to return, usually `char` for text or `unsigned char` for binary.
 *
 * This wraps a `Reader` so that developers can avoid the boilerplate of managing blocks of bytes,
 * when all they want is to iterate over the bytes of the input.
 */
template<typename T = char>
struct PerByte {
private:
    const T* ptr = nullptr;
    size_t available = 0;
    size_t current = 0;
    bool remaining = false;
    size_t overall = 0;

    Reader* reader = nullptr;

    void refill() {
        remaining = (*reader)();
        ptr = reinterpret_cast<const T*>(reader->buffer());
        available = reader->available();
        current = 0;
    }
public:
    /**
     * @param r An existing reader object that has not been read from.
     */
    PerByte(Reader& r) : reader(&r) {
        refill();
    }

    /**
     * @return Whether this instance still has bytes to be read.
     */
    bool valid() const {
        return current < available;
    }

    /**
     * Advance to the next byte, possibly reading a new chunk from the supplied `Reader`.
     * This should only be called if `valid()` is `true`.
     */
    void advance() {
        ++current;
        if (current == available) {
            overall += available;
            if (remaining) {
                refill();
            }
        }
    }

    /**
     * @return The current byte.
     *
     * This should only be called if `valid()` is `true`.
     */
    T get() const {
        return ptr[current];
    }

    /**
     * @return The position of the current byte since the start of the input.
     */
    size_t position() const {
        return overall + current;
    }
};

}

#endif
