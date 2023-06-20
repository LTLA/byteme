#ifndef BYTEME_PERBYTE_HPP
#define BYTEME_PERBYTE_HPP

/**
 * @file PerByte.hpp
 * @brief Perform byte-by-byte extraction.
 */

#include <thread>
#include <vector>
#include <algorithm>
#include <exception>
#include <type_traits>

#include "Reader.hpp"

namespace byteme {

/**
 * @brief Perform byte-by-byte extraction from a `Reader` source.
 *
 * @tparam Type_ Character type to return, usually `char` for text or `unsigned char` for binary.
 * @tparam Pointer_ A (possibly smart) non-`const` pointer to a `Reader`.
 *
 * This wraps a `Reader` so that developers can avoid the boilerplate of managing blocks of bytes,
 * when all they want is to iterate over the bytes of the input.
 */
template<typename Type_ = char, class Pointer_ = Reader*>
struct PerByte {
private:
    const Type_* ptr = nullptr;
    size_t available = 0;
    size_t current = 0;
    bool remaining = false;
    size_t overall = 0;

    Pointer_ reader;

    void refill() {
        remaining = (*reader)();
        ptr = reinterpret_cast<const Type_*>(reader->buffer());
        available = reader->available();
        current = 0;
    }
public:
    /**
     * @param r A (possibly smart) pointer to a `Reader` object that has not been read from.
     */
    PerByte(Pointer_ r) : reader(std::move(r)) {
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
     *
     * @return Whether this instance still has bytes to be read, i.e., the output of `valid()` after advancing to the next byte.
     */
    bool advance() {
        ++current;
        if (current < available) {
            return true;
        }

        overall += available;
        if (!remaining) {
            return false;
        }

        refill();
        return true;
    }

    /**
     * @return The current byte.
     *
     * This should only be called if `valid()` is `true`.
     */
    Type_ get() const {
        return ptr[current];
    }

    /**
     * @return The position of the current byte since the start of the input.
     */
    size_t position() const {
        return overall + current;
    }
};

/**
 * @brief Perform parallelized byte-by-byte extraction from a `Reader` source.
 *
 * @tparam Type_ Character type to return, usually `char` for text or `unsigned char` for binary.
 * @tparam Pointer_ A (possibly smart) non-`const` pointer to a `Reader`.
 *
 * This is much like `PerByte` except that the `Reader`'s loading operation is called in a separate thread,
 * thus allowing the caller to parse the bytes of the current chunk in parallel.
 */
template<typename Type_ = char, class Pointer_ = Reader*>
struct PerByteParallel {
private:
    size_t current = 0;
    bool remaining = false;
    size_t available = 0;
    size_t overall = 0;

    Pointer_ reader;

    bool use_meanwhile = false;
    std::thread meanwhile;
    std::exception_ptr thread_err = nullptr;
    std::vector<Type_> buffer;

    void refill() {
        auto ptr = reinterpret_cast<const Type_*>(reader->buffer());
        available = reader->available();
        buffer.resize(available);
        std::copy(ptr, ptr + available, buffer.begin());

        current = 0;
        use_meanwhile = remaining;
        if (remaining) {
            meanwhile = std::thread([&]() -> void {
                try {
                    remaining = (*reader)(); 
                } catch (...) {
                    thread_err = std::current_exception();
                }
            });
        }
    }

public:
    /**
     * @copydoc PerByte::PerByte()
     */
    PerByteParallel(Pointer_ r) : reader(std::move(r)) {
        remaining = (*reader)(); 
        refill();
    }

    /**
     * @cond
     */
    ~PerByteParallel() {
        // Avoid a dangling thread if we need to destroy this prematurely,
        // e.g., because the caller encountered an exception.
        if (use_meanwhile) {
            meanwhile.join();
        }
    }
    /**
     * @endcond
     */

    /**
     * @copydoc PerByte::valid()
     */
    bool valid() const {
        return current < available;
    }

    /**
     * @copydoc PerByte::advance()
     */
    bool advance() {
        ++current;
        if (current < available) {
            return true;
        }

        overall += available;
        if (!use_meanwhile) {
            return false;
        }

        meanwhile.join();
        if (thread_err) {
            std::rethrow_exception(thread_err);
        }
        refill();
        return true;
    }

    /**
     * @copydoc PerByte::get()
     */
    Type_ get() const {
        return buffer[current];
    }

    /**
     * @copydoc PerByte::position()
     */
    size_t position() const {
        return overall + current;
    }
};

}

#endif
