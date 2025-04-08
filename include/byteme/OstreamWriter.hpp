#ifndef BYTEME_OSTREAM_WRITER_HPP
#define BYTEME_OSTREAM_WRITER_HPP

#include <memory>
#include <stdexcept>
#include "Writer.hpp"

/**
 * @file OstreamWriter.hpp
 *
 * @brief Write bytes to an arbitrary output stream.
 */

namespace byteme {

/**
 * @brief Read bytes from a `std::ostream`.
 *
 * @tparam Stream_ Class providing an output stream of bytes, satisfying the `std::ostream` interface.
 *
 * This is just a wrapper around `std::ostream::write` for compatibility.
 */
template<class Stream_>
class OstreamWriter final : public Writer {
public:
    /**
     * @param output Pointer to an output stream.
     */
    OstreamWriter(std::unique_ptr<Stream_> output) : my_output(std::move(output)) {}

public:
    using Writer::write;

    void write(const unsigned char* buffer, size_t n) {
        my_output->write(reinterpret_cast<const char*>(buffer), n);
        if (!(my_output->good())) {
            throw std::runtime_error("failed to write to arbitrary output stream");
        }
    }

    void finish() {
        my_output->flush();
        if (my_output->fail() || my_output->bad()) {
            throw std::runtime_error("failed to flush to arbitrary output stream");
        }
    }

private:
    std::unique_ptr<Stream_> my_output;
};

}

#endif
