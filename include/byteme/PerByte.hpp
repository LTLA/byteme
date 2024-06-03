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
 * @cond
 */
template<class Pointer_>
void skip_zero_buffers(Pointer_& reader, size_t& available) {
    available = 0;
    while (reader->load()) {
        available = reader->available(); // continue collecting bytes if a zero-length buffer is returned without load() returning false.
        if (available) {
            break;
        }
    } 

    // If available == 0 on return, then reader->load() must be false,
    // and there are no more bytes left in the source.
}
/**
 * @endcond
 */

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
class PerByte {
private:
    const Type_* my_ptr = nullptr;
    size_t my_available = 0;
    size_t my_current = 0;
    size_t my_overall = 0;

    Pointer_ my_reader;

    void refill() {
        skip_zero_buffers(my_reader, my_available);
        my_ptr = reinterpret_cast<const Type_*>(my_reader->buffer());
        my_current = 0;
    }

public:
    /**
     * @param r A (possibly smart) pointer to a `Reader` object.
     */
    PerByte(Pointer_ reader) : my_reader(std::move(reader)) {
        refill();
    }

    /**
     * @return Whether this instance still has bytes to be read.
     */
    bool valid() const {
        return my_current < my_available;
    }

    /**
     * Advance to the next byte, possibly reading a new chunk from the supplied `Reader`.
     * This should only be called if `valid()` is `true`.
     *
     * @return Whether this instance still has bytes to be read, i.e., the output of `valid()` after advancing to the next byte.
     */
    bool advance() {
        ++my_current;
        if (my_current < my_available) {
            return true;
        }

        my_overall += my_available;
        refill();
        return my_available > 0; // Check that we haven't reached the end of the reader.
    }

    /**
     * @return The current byte.
     *
     * This should only be called if `valid()` is `true`.
     */
    Type_ get() const {
        return my_ptr[my_current];
    }

    /**
     * @return The position of the current byte since the start of the input.
     */
    size_t position() const {
        return my_overall + my_current;
    }

    /**
     * Extract up to `number` bytes from the `Reader` source and store them in the `output`.
     * This is equivalent to (but more efficient than) calling `get()` and then `advance()` up to `number` times,
     * only iterating while the return value of `advance()` is still true.
     * The number of successful iterations is returned in the output as the first pair element,
     * while the return value of the final `advance()` is returned as the second pair element.
     *
     * @param number Number of bytes to extract.
     * @param[out] output Pointer to an output buffer of length `n`.
     * This is filled with up to `n` bytes from the source.
     *
     * @return Pair containing (1) the number of bytes that were successfully read into `output`,
     * and (2) whether there are any more bytes available in the source for future `get()` or `extract()` calls.
     */
    std::pair<size_t, bool> extract(size_t number, Type_* output) {
        size_t original = number;
        bool okay = true;

        while (1) {
            auto start = my_ptr + my_current;
            auto leftover = my_available - my_current;

            if (leftover > number) {
                my_current += number;
                number = 0;
                std::copy(start, my_ptr + my_current, output);
                break;

            } else {
                number -= leftover;
                std::copy(start, my_ptr + my_available, output);
                my_overall += my_available;
                refill();

                okay = (my_available > 0);
                if (number == 0 || !okay) {
                    break;
                }
                output += leftover;
            }
        }

        return std::make_pair(original - number, okay);
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
class PerByteParallel {
private:
    size_t my_current = 0;
    size_t my_available = 0;
    size_t my_next_available = 0;
    size_t my_overall = 0;

    Pointer_ my_reader;

private:
    bool my_use_meanwhile;
    std::thread my_meanwhile;
    std::exception_ptr my_thread_err = nullptr;
    std::vector<Type_> my_buffer;

    void refill() {
        auto ptr = reinterpret_cast<const Type_*>(my_reader->buffer());
        my_available = my_next_available;

        // If next_available == 0, this must mean that we reached the end, 
        // so we don't bother spinning up a new thread to test that.
        my_use_meanwhile = my_next_available > 0;
        if (my_use_meanwhile) {
            my_buffer.resize(my_available);
            std::copy(ptr, ptr + my_available, my_buffer.begin());

            my_meanwhile = std::thread([&]() -> void {
                try {
                    skip_zero_buffers(my_reader, my_next_available);
                } catch (...) {
                    my_thread_err = std::current_exception();
                }
            });
        }

        my_current = 0;
    }

    void join_and_refill() {
        my_meanwhile.join();
        if (my_thread_err) {
            std::rethrow_exception(my_thread_err);
        }
        refill();
    }

public:
    /**
     * @copydoc PerByte::PerByte()
     */
    PerByteParallel(Pointer_ reader) : my_reader(std::move(reader)) {
        skip_zero_buffers(my_reader, my_next_available);
        refill();
    }

    /**
     * @cond
     */
    ~PerByteParallel() {
        // Avoid a dangling thread if we need to destroy this prematurely,
        // e.g., because the caller encountered an exception.
        if (my_use_meanwhile) {
            my_meanwhile.join();
        }
    }
    /**
     * @endcond
     */

    /**
     * @copydoc PerByte::valid()
     */
    bool valid() const {
        return my_current < my_available;
    }

    /**
     * @copydoc PerByte::advance()
     */
    bool advance() {
        ++my_current;
        if (my_current < my_available) {
            return true;
        }

        my_overall += my_available;
        if (!my_use_meanwhile) {
            return false;
        }
        join_and_refill();

        return my_available > 0; // confirm there's actually bytes to extract in the next round.
    }

    /**
     * @copydoc PerByte::get()
     */
    Type_ get() const {
        return my_buffer[my_current];
    }

    /**
     * @copydoc PerByte::position()
     */
    size_t position() const {
        return my_overall + my_current;
    }

    /**
     * Extract up to `number` bytes from the `Reader` source and store them in the `output`.
     * This is equivalent to (but more efficient than) calling `get()` and then `advance()` up to `number` times,
     * only iterating while the return value of `advance()` is still true.
     * The number of successful iterations is returned in the output as the first pair element,
     * while the return value of the final `advance()` is returned as the second pair element.
     *
     * @param n Number of bytes to extract.
     * @param[out] output Pointer to an output buffer of length `n`.
     * This is filled with up to `n` bytes from the source.
     *
     * @return Pair containing (1) the number of bytes that were successfully read into `output`,
     * and (2) whether there are any more bytes available in the source for future `get()` or `extract()` calls.
     */
    std::pair<size_t, bool> extract(size_t number, Type_* output) {
        size_t original = number;
        bool okay = true;

        while (1) {
            auto start = my_buffer.data() + my_current;
            auto leftover = my_available - my_current;

            if (leftover > number) {
                my_current += number;
                number = 0;
                std::copy(start, my_buffer.data() + my_current, output);
                break;

            } else {
                number -= leftover;
                std::copy(start, my_buffer.data() + my_available, output);

                my_overall += my_available;
                if (!my_use_meanwhile) {
                    okay = false;
                    break;
                }
                join_and_refill();

                okay = (my_available > 0);
                if (number == 0 || !okay) {
                    break;
                }
                output += leftover;
            }
        }

        return std::make_pair(original - number, okay);
    }
};

}

#endif
