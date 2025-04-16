#ifndef BYTEME_OSTREAM_WRITER_HPP
#define BYTEME_OSTREAM_WRITER_HPP

#include <memory>
#include <stdexcept>
#include <cstddef>

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
 * This is most typically a `std::unique_ptr<std::ostream> >` but a concrete subclass may also be used to encourage compiler devirtualization.
 * Either a raw or smart pointer may be used depending on how the lifetime of the pointed-to object is managed.
 *
 * This is just a wrapper around `std::ostream::write` for compatibility.
 */
template<class Pointer_>
class OstreamWriter final : public Writer {
public:
    /**
     * @param output Pointer to an output stream.
     */
    OstreamWriter(Pointer_ output) : my_output(std::move(output)) {}

public:
    using Writer::write;

    void write(const unsigned char* buffer, std::size_t n) {
        safe_write<std::streamsize, false>(
            reinterpret_cast<const char*>(buffer),
            n,
            [&](const char* ptr0, std::streamsize n0) -> void {
                my_output->write(ptr0, n0);
                if (!(my_output->good())) {
                    throw std::runtime_error("failed to write to arbitrary output stream");
                }
            }
        );
    }

    void finish() {
        my_output->flush();
        if (my_output->fail() || my_output->bad()) {
            throw std::runtime_error("failed to flush to arbitrary output stream");
        }
    }

private:
    Pointer_ my_output;
};

}

#endif
