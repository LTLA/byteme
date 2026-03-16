#ifndef BYTEME_BUFFERED_READER_HPP
#define BYTEME_BUFFERED_READER_HPP

/**
 * @file BufferedReader.hpp
 * @brief Buffered wrapper around a `Reader`.
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
 * @brief Buffered wrapper around a `Reader`.
 *
 * @tparam Type_ Type of the output bytes, usually `char` for text or `unsigned char` for binary.
 *
 * In some applications, we may need to iterate over many small chunks of bytes or even individual bytes.
 * Naively calling `Reader::read()` for each request may be inefficient if the underlying implementation needs to read from some storage device for each call.
 * Instead, we can wrap our `Reader` in a `BufferedReader` instance, which calls `Reader::read()` every now and then to fill a large intermediate buffer.
 * Users can then iterate that buffer to obtain the next byte or chunk of bytes, reducing the number of separate calls to `Reader::read()`.
 *
 * Check out `SerialBufferedReader` and `ParallelBufferedReader` for subclasses.
 */
template<typename Type_>
class BufferedReader {
public:
    /**
     * @cond
     */
    BufferedReader() = default;
    virtual ~BufferedReader() = default;

    BufferedReader(BufferedReader&&) = delete;
    BufferedReader(const BufferedReader&) = delete; // not copyable.
    BufferedReader& operator=(BufferedReader&&) = delete;
    BufferedReader& operator=(const BufferedReader&) = delete; // not copyable.
    /**
     * @endcond
     */

protected:
    /**
     * @cond
     */
    virtual std::pair<const Type_*, std::size_t> refill() = 0;

    virtual std::size_t refill(Type_*) = 0;

    std::size_t buffer_size;

    void initialize() {
        auto rout = refill();
        my_buffer_ptr = rout.first;
        my_available = rout.second;
    }
    /**
     * @endcond
     */

private:
    const Type_* my_buffer_ptr;
    std::size_t my_available = 0;
    std::size_t my_current = 0;

    // Standard guarantees this to be at least 64 bits, which is more than that of size_t.
    // We don't use uint64_t because that might not be defined by the implementation.
    unsigned long long my_overall = 0;

public:
    /**
     * Advance to the next byte, possibly refilling the buffer using bytes from the supplied `Reader`.
     * This should only be called if `valid()` is `true`.
     *
     * @return Whether the buffer still has one or more bytes that can be read, i.e., the output of `valid()` after advancing to the next byte.
     */
    bool advance() {
        ++my_current;
        if (my_current < my_available) {
            return true;
        }

        my_current = 0;
        my_overall += my_available;

        if (my_available == buffer_size) {
            const auto rout = refill();
            my_buffer_ptr = rout.first;
            my_available = rout.second;
        } else {
            my_available = 0;
        }

        return my_available > 0; // Check that we haven't reached the end of the reader.
    }

    /**
     * @return The current byte.
     *
     * This should only be called if `valid()` is `true`.
     */
    Type_ get() const {
        return my_buffer_ptr[my_current];
    }

    /**
     * @return The position of the current byte since the start of the input.
     */
    unsigned long long position() const {
        return my_overall + my_current;
    }

    /**
     * @return Whether the buffer still has one or more bytes that can be read.
     */
    bool valid() const {
        return my_current < my_available;
    }

public:
    /**
     * Extract up to `number` bytes from the buffer and store them in the `output`.
     * This is equivalent to (but more efficient than) calling `get()` and then `advance()` up to `number` times,
     * only iterating while the return value of `advance()` is still true.
     * Users should only call this method if `valid()` is still true.
     *
     * @param number Number of bytes to extract.
     * @param[out] output Pointer to an output buffer of length `number`.
     * This is filled with up to `number` bytes from the source.
     *
     * @return Pair containing:
     *
     * - The number of bytes that were successfully read into `output`.
     *   This can also be interpreted as the number of successful `get()`/`advance()` iterations.
     * - Wether there are any more bytes available in the source for future `get()` or `extract()` calls.
     *   This can also be interpreted as the result of the final `advance()` (i.e., the result of the `valid()` after `extract()` returns).
     *
     * If the first element is less than `number`, it can be assumed that no more bytes are available in the source (i.e., the second element must be false).
     * Note that the converse is not true as it is possible to read `number` bytes and finish the source at the same time.
     */
    std::pair<std::size_t, bool> extract(std::size_t number, Type_* output) {
        const std::size_t original = number;

        // Copying contents of the cached buffer. 
        const std::size_t remaining = my_available - my_current;
        if (number < remaining) {
            std::copy_n(my_buffer_ptr + my_current, number, output); 
            my_current += number;
            return std::make_pair(number, true);
        }

        std::copy_n(my_buffer_ptr + my_current, remaining, output); 
        output += remaining;
        number -= remaining;

        // If the available number of bytes is less than the buffer size, the reader is already exhausted.
        if (my_available < buffer_size) {
            my_current = my_available;
            return std::make_pair(original - number, false);
        }

        // Directly filling the output array, bypassing our buffer.
        while (number >= buffer_size) {
            my_overall += my_available;
            my_available = refill(output);

            output += my_available;
            number -= my_available;
            if (my_available < buffer_size) {
                my_current = my_available;
                return std::make_pair(original - number, false);
            }
        }

        // Filling the cache and copying the remainder into the output array.
        my_overall += my_available;
        auto rout = refill();
        my_buffer_ptr = rout.first;
        my_available = rout.second;

        const auto to_use = std::min(my_available, number);
        std::copy_n(my_buffer_ptr, to_use, output);
        number -= to_use;
        my_current = to_use;

        // We know that number < buffer_size from the loop condition, so if exhausted == true,
        // this means that my_available <= number < buffer_size, i.e., the source is exhausted.
        // Thus, any future advance() would definitely return false.
        bool exhausted = (to_use == my_available);

        return std::make_pair(original - number, !exhausted);
    }
};

/**
 * @brief Serial buffering to wrap a `Reader`.
 *
 * @tparam Type_ Type of the output bytes, usually `char` for text or `unsigned char` for binary.
 * @tparam Pointer_ Pointer to a class that serves as a source of input bytes.
 * The pointed-to class should satisfy the `Reader` interface; it may also be a concrete `Reader` subclass to enable devirtualization. 
 * Either a smart or raw pointer may be supplied depending on how the caller wants to manage the lifetime of the pointed-to object. 
 *
 * This is a subclass of `BufferedReader` than calls `Reader::read()` on the main thread.
 */
template<typename Type_, class Pointer_ = std::unique_ptr<Reader> >
class SerialBufferedReader final : public BufferedReader<Type_> {
public:
    /**
     * @param reader Pointer to a source of input bytes.
     * @param buffer_size Size of the internal buffer in which to store bytes that have been read from `reader` but not yet used.
     * Larger values reduce the number of reader calls at the cost of greater memory usage.
     */
    SerialBufferedReader(Pointer_ reader, std::size_t buffer_size) : my_reader(std::move(reader)), my_buffer(buffer_size) {
        this->buffer_size = buffer_size;
        this->initialize();
    }

private:
    Pointer_ my_reader;
    std::vector<Type_> my_buffer;

protected:
    /**
     * @cond
     */
    std::pair<const Type_*, std::size_t> refill() {
        auto ptr = my_buffer.data();
        return std::make_pair(ptr, refill(ptr));
    }

    std::size_t refill(Type_* ptr) {
        return my_reader->read(reinterpret_cast<unsigned char*>(ptr), this->buffer_size);
    }
    /**
     * @endcond
     */
};

/**
 * @brief Parallelized buffering to wrap a `Reader`.
 *
 * @tparam Type_ Type of the output bytes, usually `char` for text or `unsigned char` for binary.
 * @tparam Pointer_ Pointer to a class that serves as a source of input bytes.
 * The pointed-to class should satisfy the `Reader` interface; it may also be a concrete `Reader` subclass to enable devirtualization. 
 * Either a smart or raw pointer may be supplied depending on how the caller wants to manage the lifetime of the pointed-to object. 
 *
 * This is a subclass of `BufferedReader` than calls `Reader::read()` on a worker thread.
 * The user can iterate over the currently-filled buffer in parallel with the readiing of the next buffer's worth of bytes.
 */
template<typename Type_, class Pointer_ = std::unique_ptr<Reader> >
class ParallelBufferedReader final : public BufferedReader<Type_> {
public:
    /**
     * @param reader Pointer to a source of input bytes.
     * @param buffer_size Size of the internal buffer in which to store bytes that have been read from `reader` but not yet used.
     * Larger values reduce the number of reader calls at the cost of greater memory usage.
     */
    ParallelBufferedReader(Pointer_ reader, std::size_t buffer_size) : my_reader(std::move(reader)), my_buffer_main(buffer_size), my_buffer_worker(buffer_size) {
        this->buffer_size = buffer_size;
        my_thread = std::thread([&]() { thread_loop(); }); // set up thread before initializing.
        this->initialize();
    }

    /**
     * @cond
     */
    ~ParallelBufferedReader() {
        std::unique_lock lck(my_mut);
        my_kill = true;
        my_ready_input = true;
        lck.unlock(); // releasing the lock so that the notified thread doesn't immediately block.
        my_cv.notify_one();
        my_thread.join();
    }
    /**
     * @endcond
     */

private:
    Pointer_ my_reader;
    std::vector<Type_> my_buffer_main, my_buffer_worker;
    std::size_t my_next_available = 0;

private:
    std::thread my_thread;
    std::exception_ptr my_thread_err = nullptr;
    std::mutex my_mut;
    std::condition_variable my_cv;

    bool my_ready_input = false, my_ready_output = false;
    bool my_worker_active = false;
    bool my_kill = false;

    void thread_loop() {
        while (1) {
            std::unique_lock lck(my_mut);
            my_cv.wait(lck, [&]() { return my_ready_input; });
            my_ready_input = false;

            if (my_kill) { // Handle an explicit kill signal from the destructor.
                break;
            }

            try {
                my_next_available = my_reader->read(reinterpret_cast<unsigned char*>(my_buffer_worker.data()), this->buffer_size);
            } catch (...) {
                my_thread_err = std::current_exception();
            }

            my_ready_output = true;
            lck.unlock();
            my_cv.notify_one();
        }
    }

protected:
    /**
     * @cond
     */
    std::pair<const Type_*, std::size_t> refill() {
        if (my_worker_active) {
            // If the worker is active, it's probably already gotten started so we just wait for it to finish.
            // Then we swap the results with the main buffer and submit a new job to the worker.
            std::unique_lock lck(my_mut);
            my_cv.wait(lck, [&]() { return my_ready_output; });
            my_ready_output = false;

            if (my_thread_err) {
                std::rethrow_exception(my_thread_err);
            }

            my_buffer_main.swap(my_buffer_worker);
            auto output = std::make_pair(my_buffer_main.data(), my_next_available);

            my_ready_input = true;
            lck.unlock();
            my_cv.notify_one();

            return output;

        } else {
            // If the worker is not active, we just do a read directly on the main thread, and then submit a new job to the worker.
            // Obviously, if we did the read on the worker, we'd have to wait for it anyway, so we might as well cut out the communication overhead.
            auto available = my_reader->read(reinterpret_cast<unsigned char*>(my_buffer_main.data()), this->buffer_size);

            std::unique_lock lck(my_mut);
            my_ready_input = true;
            lck.unlock();
            my_cv.notify_one();
            my_worker_active = true;

            return std::make_pair(my_buffer_main.data(), available);
        }
    }

    std::size_t refill(Type_* ptr) {
        if (my_worker_active) {
            // If the worker is active, we wait for it to finish, transfer the results to the supplied pointer.
            // We do not submit a new job as we'll probably want to call this overload again - see the loop in extract(). 
            // On subsequent calls, we'll want to directly read from the Reader into the supplied pointer to cut out a copy.
            // But even if we didn't call this overload again, we'd probablyprobably  call the other overload immediately - see how extract() works.
            // So if we sent a new job to the worker, we'd end up immediately waiting on it for results, so we might as well skip that.
            std::unique_lock lck(my_mut);
            my_cv.wait(lck, [&]() { return my_ready_output; });
            my_ready_output = false;
            if (my_thread_err) {
                std::rethrow_exception(my_thread_err);
            }

            std::copy_n(my_buffer_worker.begin(), my_next_available, ptr);
            my_worker_active = false;
            return my_next_available;

        } else {
            // If the worker isn't active, we can directly read the into the supplied pointer, avoiding some communication overhead.
            return my_reader->read(reinterpret_cast<unsigned char*>(ptr), this->buffer_size);
        }
    }
    /**
     * @endcond
     */
};

}

#endif
