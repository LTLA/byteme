#ifndef BYTEME_BUFFERED_WRITER_HPP
#define BYTEME_BUFFERED_WRITER_HPP

/**
 * @file BufferedWriter.hpp
 * @brief Buffered wrapper around a `Writer`.
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
#include <string>

#include "sanisizer/sanisizer.hpp"

#include "Writer.hpp"
#include "utils.hpp"

namespace byteme {

/**
 * @brief Buffered wrapper around a `Writer`.
 *
 * @tparam Type_ Type of the input bytes, usually `char` for text or `unsigned char` for binary.
 *
 * In some applications, we may need to write many small chunks of bytes or even individual bytes.
 * Naively calling `Writer::write()` for each request may be inefficient if the underlying implementation attempts to write to some storage device for each call.
 * Instead, we can wrap our `Writer` in a `BufferedWriter` instance, which stores user-supplied data into a large intermediate buffer.
 * It then calls `Writer::write()` every now and then to flush the buffer to the underlying representation.
 *
 * Check out `SerialBufferedWriter` and `ParallelBufferedWriter` for subclasses.
 */
template<typename Type_, class WriterPointer_ = std::unique_ptr<Writer> >
class BufferedWriter {
public:
    /**
     * @cond
     */
    BufferedWriter(std::size_t buffer_size) : my_buffer(sanisizer::cast<I<decltype(my_buffer.size())> >(buffer_size)) {
        if (buffer_size == 0) {
            throw std::runtime_error("buffer size must be positive");
        }
    }

    BufferedWriter(BufferedWriter&&) = delete;
    BufferedWriter(const BufferedWriter&) = delete; // not copyable.
    BufferedWriter& operator=(BufferedWriter&&) = delete;
    BufferedWriter& operator=(const BufferedWriter&) = delete; // not copyable.

    virtual ~BufferedWriter() = default;
    /**
     * @endcond
     */

protected:
    /**
     * @cond
     */
    virtual void flush_async() = 0;

    virtual void flush_sync(const Type_*, std::size_t) = 0;

    std::vector<Type_>& get_buffer() {
        return my_buffer;
    }
    /**
     * @endcond
     */

private:
    std::vector<Type_> my_buffer;
    std::size_t my_current = 0;

    // Standard guarantees this to be at least 64 bits, which is more than that of size_t.
    // We don't use uint64_t because that might not be defined by the implementation.
    unsigned long long my_overall = 0;

public:
    /**
     * @return Number of bytes that have currently been written via calls to `write()`.
     */
    unsigned long long number() {
        return my_overall + my_current;
    }

public:
    /**
     * Write a single byte to the buffer.
     * This may trigger a call to `flush()`.
     *
     * @param input Byte to be written.
     */
    void write(Type_ input) {
        my_buffer[my_current] = input;
        ++my_current;

        const auto buffer_size = my_buffer.size();
        if (my_current == buffer_size) {
            flush_async();
            my_current = 0;
            my_overall += buffer_size;
        }
    }

    /**
     * Write an array of bytes to the buffer.
     * This may trigger a multiple calls to `flush()`.
     *
     * @param[in] input Pointer to an array of bytes to be written.
     * @param number Length of the array in `input`.
     */
    void write(const Type_* input, std::size_t number) {
        const auto buffer_size = my_buffer.size();

        // Seeing if we fill up the rest of our buffer or not.
        const std::size_t remaining = buffer_size - my_current;
        if (number < remaining) {
            std::copy_n(input, number, my_buffer.data() + my_current); 
            my_current += number;
            return;
        }

        std::copy_n(input, remaining, my_buffer.data() + my_current); 
        number -= remaining;
        input += remaining;

        if (number == 0) {
            flush_async();
            my_overall += buffer_size;
            my_current = 0;
            return;
        }

        flush_sync(my_buffer.data(), buffer_size);
        my_overall += buffer_size;

        // Directly filling the output array, bypassing our buffer.
        while (number >= buffer_size) {
            flush_sync(input, buffer_size);
            my_overall += buffer_size;
            input += buffer_size;
            number -= buffer_size;
        } 

        // Filling the cache and copying the remainder into the output array.
        std::copy_n(input, number, my_buffer.data());
        my_current = number;
    }

    /**
     * Write a null-terminated string to the buffer.
     * This is a convenient overload to the array `write()` and may trigger a call to `flush()`.
     *
     * @param string Pointer to a null-terminated string.
     */
    void write(const char* string) {
        write(reinterpret_cast<const Type_*>(string), std::char_traits<char>::length(string));
    }

public:
    /**
     * Flush all currently-buffered bytes to the underlying `Writer` instance.
     * This can be called at any time, before or after other calls to `write()` or `flush()`.
     * It is also automatically called upon `BufferedWriter` destruction.
     */
    void flush() {
        flush_sync(my_buffer.data(), my_current); // Flush whatever's left.
        my_overall += my_current;
        my_current = 0;
    }

    /**
     * Flush all currently-buffered bytes to the underlying `Writer` instance and call `Writer::finish()`.
     * This terminates the write stream so no further calls to `write()`, `flush()` or `finish()` may be performed.
     * It is not called automatically upon `BufferedWriter` destruction,
     * as the user may want to write further bytes to the same `Writer` instance after exiting the `BufferedWriter` wrapper.
     */
    virtual void finish() = 0;
};

/**
 * @brief Serial buffering to wrap a `Writer`.
 *
 * @tparam Type_ Type of the output bytes, usually `char` for text or `unsigned char` for binary.
 * @tparam Pointer_ Pointer to a class that serves as a source of input bytes.
 * The pointed-to class should satisfy the `Writer` interface; it may also be a concrete `Writer` subclass to enable devirtualization. 
 * Either a smart or raw pointer may be supplied depending on how the caller wants to manage the lifetime of the pointed-to object. 
 *
 * This is a subclass of `BufferedWriter` that calls `Writer::write()` on the main thread.
 */
template<typename Type_, class Pointer_ = std::unique_ptr<Writer> >
class SerialBufferedWriter final : public BufferedWriter<Type_> {
public:
    /**
     * @param writer Pointer to a source of input bytes.
     * @param buffer_size Size of the internal buffer in which to store bytes that have been passed to `write()` but not yet flushed to the `writer`. 
     * Larger values reduce the number of write calls at the cost of greater memory usage.
     * Should be positive.
     */
    SerialBufferedWriter(Pointer_ writer, std::size_t buffer_size) : BufferedWriter<Type_>(buffer_size), my_writer(std::move(writer)) {}

    /**
     * @cond
     */
    ~SerialBufferedWriter() {
        this->flush();
    }
    /**
     * @endcond
     */

private:
    Pointer_ my_writer;

protected:
    /**
     * @cond
     */
    void flush_async() {
        auto& buffer = this->get_buffer();
        my_writer->write(reinterpret_cast<unsigned char*>(buffer.data()), buffer.size());
    }

    void flush_sync(const Type_* ptr, std::size_t num) {
        my_writer->write(reinterpret_cast<const unsigned char*>(ptr), num);
    }

    void finish() {
        this->flush();
        my_writer->finish();
    }
    /**
     * @endcond
     */
};

/**
 * @brief Parallelized buffering to wrap a `Writer`.
 *
 * @tparam Type_ Type of the output bytes, usually `char` for text or `unsigned char` for binary.
 * @tparam Pointer_ Pointer to a class that serves as a source of input bytes.
 * The pointed-to class should satisfy the `Writer` interface; it may also be a concrete `Writer` subclass to enable devirtualization. 
 * Either a smart or raw pointer may be supplied depending on how the caller wants to manage the lifetime of the pointed-to object. 
 *
 * This is a subclass of `BufferedWriter` that calls `Writer::write()` on a worker thread.
 * The user can fill up the next buffer while the current buffer is flushed to the underlying representation.
 */
template<typename Type_, class Pointer_ = std::unique_ptr<Writer> >
class ParallelBufferedWriter final : public BufferedWriter<Type_> {
public:
    /**
     * @param writer Pointer to a source of input bytes.
     * @param buffer_size Size of the internal buffer in which to store bytes that have been passed to `write()` but not yet flushed to the `writer`. 
     * Larger values reduce the number of write calls at the cost of greater memory usage.
     * Should be positive.
     */
    ParallelBufferedWriter(Pointer_ writer, std::size_t buffer_size) : 
        BufferedWriter<Type_>(buffer_size),
        my_writer(std::move(writer)),
        my_buffer_worker(sanisizer::cast<I<decltype(my_buffer_worker.size())> >(buffer_size))
    {
        my_thread = std::thread([&]() { thread_loop(); }); // set up thread before initializing.
    }

    /**
     * @cond
     */
    ~ParallelBufferedWriter() {
        this->flush();

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
    Pointer_ my_writer;
    std::vector<Type_> my_buffer_worker;
    std::size_t my_to_write = 0;

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
                my_writer->write(reinterpret_cast<const unsigned char*>(my_buffer_worker.data()), my_to_write);
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
    void flush_async() {
        auto& buffer = this->get_buffer();
        const auto num = buffer.size();

        if (my_worker_active) {
            // If the worker is active, it's probably already gotten started so we just wait for it to finish.
            // Then we swap the results with the main buffer and submit a new job to the worker.
            std::unique_lock lck(my_mut);
            my_cv.wait(lck, [&]() { return my_ready_output; });
            my_ready_output = false;

            if (my_thread_err) {
                std::rethrow_exception(my_thread_err);
            }

            buffer.swap(my_buffer_worker);
            my_to_write = num;

            my_ready_input = true;
            lck.unlock();
            my_cv.notify_one();

        } else {
            // If the worker is not active, we can just submit directly.
            std::unique_lock lck(my_mut);
            buffer.swap(my_buffer_worker);
            my_to_write = num;

            my_ready_input = true;
            lck.unlock();
            my_cv.notify_one();
            my_worker_active = true;
        }
    }

    void flush_sync(const Type_* ptr, std::size_t num) {
        if (my_worker_active) {
            // If the worker is active, we wait for it to finish and then write from the supplied pointer.
            // We do not submit a new job, based on the loop in the BufferedWriter::write() overload:
            // 
            // - We'll probably want to call this flush() overload again.
            //   On subsequent calls, we'll want to directly write from the supplied pointer to the Writer to cut out a copy.
            // - If we didn't call this overload again, we'd probably call the other refill() overload immediately, 
            //   So if we sent a new job to the worker, we'd end up immediately blocking on it to submit the next write.
            //   So we might as well skip that communication overhead and write directly on the main thread.
            std::unique_lock lck(my_mut);
            my_cv.wait(lck, [&]() { return my_ready_output; });
            my_ready_output = false;
            if (my_thread_err) {
                std::rethrow_exception(my_thread_err);
            }

            my_writer->write(reinterpret_cast<const unsigned char*>(ptr), num);
            my_worker_active = false;

        } else {
            // If the worker isn't active, we can directly write from the supplied pointer, avoiding some communication overhead.
            my_writer->write(reinterpret_cast<const unsigned char*>(ptr), num);
        }
    }

    void finish() {
        this->flush();
        my_writer->finish();
    }
    /**
     * @endcond
     */
};

}

#endif
