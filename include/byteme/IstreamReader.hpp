#ifndef BYTEME_ISTREAM_READER_HPP
#define BYTEME_ISTREAM_READER_HPP

#include <vector>
#include <stdexcept>
#include <memory>
#include <cstddef>

#include "Reader.hpp"
#include "check_buffer_size.hpp"

/**
 * @file IstreamReader.hpp
 *
 * @brief Read bytes from an input stream.
 */

namespace byteme {

/**
 * @brief Options for `IstreamReader` construction.
 */
struct IstreamReaderOptions {
    /**
     * Size of the internal buffer to fill from the stream.
     * Larger values usually reduce computational time at the cost of increased memory usage.
     */
    std::size_t buffer_size = 65536;
};

/**
 * @brief Read bytes from a `std::istream`.
 *
 * @tparam Pointer_ Pointer to a class providing an input stream of bytes, satisfying the `std::istream` interface.
 * This is most typically a `std::unique_ptr<std::istream> >` but a concrete subclass may also be used to encourage compiler devirtualization.
 * Either a raw or smart pointer may be used depending on how the lifetime of the pointed-to object is managed.
 *
 * This is just a wrapper around `std::istream::read`,
 * mostly to avoid having to remember the correct way to check for end of file.
 */
template<class Pointer_>
class IstreamReader final : public Reader {
public:
    /**
     * @param input Pointer to an input stream.
     * @param options Further options.
     */
    IstreamReader(Pointer_ input, const IstreamReaderOptions& options) : 
        my_input(std::move(input)),
        my_buffer(
            check_buffer_size<std::streamsize>( // for istream::read().
                check_buffer_size(options.buffer_size)
            )
        )
    {}

public:
    bool load() {
        if (!my_okay) {
            return false;
        }

        my_input->read(reinterpret_cast<char*>(my_buffer.data()), my_buffer.size());
        my_read = my_input->gcount();

        if (my_read < my_buffer.size()) {
            if (my_input->eof()) {
                my_okay = false;
            } else {
                throw std::runtime_error("failed to finish reading the input stream");
            }
        }

        return true;
    }

    const unsigned char* buffer() const {
        return my_buffer.data();
    }

    std::size_t available() const {
        return my_read;
    }

private:
    Pointer_ my_input;
    std::vector<unsigned char> my_buffer;
    std::size_t my_read = 0;
    bool my_okay = true;
};

}

#endif
