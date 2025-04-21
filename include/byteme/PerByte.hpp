#ifndef BYTEME_PERBYTE_HPP
#define BYTEME_PERBYTE_HPP

/**
 * @file PerByte.hpp
 * @brief Perform byte-by-byte extraction.
 */

#include <thread>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <algorithm>
#include <exception>
#include <type_traits>
#include <memory>
#include <cstddef>

#include "Reader.hpp"

namespace byteme {

/**
 * @cond
 */
template<class Reader_>
void skip_zero_buffers(Reader_& reader, std::size_t& available) {
    available = 0;
    while (reader.load()) {
        available = reader.available(); // continue collecting bytes if a zero-length buffer is returned without load() returning false.
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
 * @brief Interface for byte-by-byte extraction from a `Reader` source.
 *
 * @tparam Type_ Type of the output bytes, usually `char` for text or `unsigned char` for binary.
 *
 * This wraps a `Reader` so that developers can avoid the boilerplate of managing blocks of bytes,
 * when all they want is to iterate over the bytes of the input.
 */
template<typename Type_>
class PerByteInterface {
public:
    /**
     * @cond
     */
    PerByteInterface() = default;
    PerByteInterface(PerByteInterface&&) = delete;
    PerByteInterface(const PerByteInterface&) = delete; // not copyable.
    PerByteInterface& operator=(PerByteInterface&&) = delete;
    PerByteInterface& operator=(const PerByteInterface&) = delete; // not copyable.
    virtual ~PerByteInterface() = default;
    /**
     * @endcond
     */

protected:
    /**
     * Pointer to an array of bytes, to be set by `refill()` whenever `available > 0`.
     */
    const Type_* ptr = nullptr;

    /**
     * Length of the array at `ptr`.
     */
    std::size_t available = 0;

    /**
     * Set `ptr` and `available` to refer to an array of new bytes from a `Reader`.
     */
    virtual void refill() = 0;

private:
    std::size_t my_current = 0;

    // Standard guarantees this to be at least 64 bits, which is more than that of size_t.
    // We don't use uint64_t because that might not be defined by the implementation.
    unsigned long long my_overall = 0;

public:
    /**
     * Advance to the next byte, possibly reading a new chunk from the supplied `Reader`.
     * This should only be called if `valid()` is `true`.
     *
     * @return Whether this instance still has bytes to be read, i.e., the output of `valid()` after advancing to the next byte.
     */
    bool advance() {
        ++my_current;
        if (my_current < available) {
            return true;
        }

        my_current = 0;
        my_overall += available;
        refill();
        return available > 0; // Check that we haven't reached the end of the reader.
    }

    /**
     * @return The current byte.
     *
     * This should only be called if `valid()` is `true`.
     */
    Type_ get() const {
        return ptr[my_current];
    }

    /**
     * @return The position of the current byte since the start of the input.
     */
    unsigned long long position() const {
        return my_overall + my_current;
    }

    /**
     * @return Whether this instance still has bytes to be read.
     */
    bool valid() const {
        return my_current < available;
    }

public:
    /**
     * Extract up to `number` bytes from the `Reader` source and store them in the `output`.
     * This is equivalent to (but more efficient than) calling `get()` and then `advance()` up to `number` times,
     * only iterating while the return value of `advance()` is still true.
     * The number of successful iterations is returned in the output as the first pair element,
     * while the return value of the final `advance()` is returned as the second pair element.
     *
     * @param number Number of bytes to extract.
     * @param[out] output Pointer to an output buffer of length `number`.
     * This is filled with up to `number` bytes from the source.
     *
     * @return Pair containing (1) the number of bytes that were successfully read into `output`,
     * and (2) whether there are any more bytes available in the source for future `get()` or `extract()` calls.
     * If the first element is less than `number`, it can be assumed that no more bytes are available in the source (i.e., the second element will be false).
     */
    std::pair<std::size_t, bool> extract(std::size_t number, Type_* output) {
        std::size_t original = number;
        bool okay = true;

        while (1) {
            auto start = ptr + my_current;
            auto leftover = available - my_current;

            if (leftover > number) {
                my_current += number;
                number = 0;
                std::copy(start, ptr + my_current, output);
                break;

            } else {
                number -= leftover;
                std::copy(start, ptr + available, output);

                my_current = 0;
                my_overall += available;
                refill();

                okay = (available > 0);
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
 * @brief Serial byte-by-byte extraction from a `Reader` source.
 *
 * @tparam Type_ Type of the output bytes, usually `char` for text or `unsigned char` for binary.
 * @tparam Pointer_ Pointer to a class that serves as a source of input bytes.
 * The pointed-to class should satisfy the `Reader` interface; it may also be a concrete `Reader` subclass to enable devirtualization. 
 * Either a smart or raw pointer may be supplied depending on how the caller wants to manage the lifetime of the pointed-to object. 
 *
 * This wraps a `Reader` so that developers can avoid the boilerplate of managing blocks of bytes,
 * when all they want is to iterate over the bytes of the input.
 */
template<typename Type_, class Pointer_ = std::unique_ptr<Reader> >
class PerByteSerial final : public PerByteInterface<Type_> {
public:
    /**
     * @param reader Pointer to a source of input bytes.
     */
    PerByteSerial(Pointer_ reader) : my_reader(std::move(reader)) {
        refill();
    }

private:
    Pointer_ my_reader;

protected:
    void refill() {
        skip_zero_buffers(*my_reader, this->available);
        this->ptr = reinterpret_cast<const Type_*>(my_reader->buffer());
    }
};

/**
 * @brief Parallelized byte-by-byte extraction from a `Reader` source.
 *
 * @tparam Type_ Type of the output bytes, usually `char` for text or `unsigned char` for binary.
 * @tparam Pointer_ Pointer to a class that serves as a source of input bytes.
 * The pointed-to class should satisfy the `Reader` interface; it may also be a concrete `Reader` subclass to enable devirtualization. 
 * Either a smart or raw pointer may be supplied depending on how the caller wants to manage the lifetime of the pointed-to object. 
 *
 * This is much like `PerByteSerial` except that the `Reader_`'s loading operation is called in a separate thread,
 * thus allowing the caller to parse the bytes of the current chunk in parallel.
 */
template<typename Type_, class Pointer_ = std::unique_ptr<Reader> >
class PerByteParallel final : public PerByteInterface<Type_> {
public:
    /**
     * @param reader Pointer to a source of input bytes.
     */
    PerByteParallel(Pointer_ reader) : my_reader(std::move(reader)) {
        my_ready_input = false;
        my_thread = std::thread([&]() { thread_loop(); });

        skip_zero_buffers(*my_reader, my_next_available);
        my_ready_output = true; // run the first iteration of refill().
        refill();
    }

    /**
     * @cond
     */
    ~PerByteParallel() {
        if (!my_finished) {
            std::unique_lock lck(my_mut);
            my_finished = true;
            my_ready_input = true;
            lck.unlock(); // releasing the lock so that the notified thread doesn't immediately block.
            my_cv.notify_one();
        }
        my_thread.join();
    }
    /**
     * @endcond
     */

private:
    Pointer_ my_reader;
    std::vector<Type_> my_buffer;
    std::size_t my_next_available = 0;
    bool my_finished = false;

private:
    std::thread my_thread;
    std::exception_ptr my_thread_err = nullptr;
    std::mutex my_mut;
    std::condition_variable my_cv;
    bool my_ready_input, my_ready_output;

    void thread_loop() {
        while (!my_finished) {
            std::unique_lock lck(my_mut);
            my_cv.wait(lck, [&]() { return my_ready_input; });
            my_ready_input = false;

            if (my_finished) { // an explicit kill signal from the destructor.
                break;
            }

            try {
                skip_zero_buffers(*my_reader, my_next_available);
                my_finished = my_next_available == 0; // see the definition of skip_zero_buffers().
            } catch (...) {
                my_thread_err = std::current_exception();
                my_finished = true;
            }

            my_ready_output = true;
            lck.unlock();
            my_cv.notify_one();
        }
    }

protected:
    void refill() {
        if (my_finished) {
            this->ptr = nullptr;
            this->available = 0;
            return;
        }

        std::unique_lock lck(my_mut);
        my_cv.wait(lck, [&]() { return my_ready_output; });
        my_ready_output = false;
        if (my_thread_err) {
            std::rethrow_exception(my_thread_err);
        }

        auto rptr = reinterpret_cast<const Type_*>(my_reader->buffer());
        this->available = my_next_available;
        my_buffer.resize(this->available);
        std::copy_n(rptr, this->available, my_buffer.data());
        this->ptr = my_buffer.data();

        my_ready_input = true;
        lck.unlock();
        if (!my_finished) {
            my_cv.notify_one();
        }
    }
};

}

#endif
