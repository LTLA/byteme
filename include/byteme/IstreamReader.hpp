#ifndef BYTEME_ISTREAM_READER_HPP
#define BYTEME_ISTREAM_READER_HPP

#include <vector>
#include <stdexcept>
#include <memory>
#include <cstddef>

#include "Reader.hpp"
#include "utils.hpp"

/**
 * @file IstreamReader.hpp
 *
 * @brief Read bytes from an input stream.
 */

namespace byteme {

/**
 * @brief Read bytes from a `std::istream`.
 *
 * @tparam Pointer_ Pointer to a class providing an input stream of bytes, satisfying the `std::istream` interface.
 * This is most typically a `std::unique_ptr<std::istream>` but a pointer to a concrete subclass may also be used to encourage devirtualization.
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
    IstreamReader(Pointer_ input) : my_input(std::move(input)) {}

public:
    std::size_t read(unsigned char* buffer, std::size_t n) {
        return safe_read<std::streamsize>(buffer, n, [this](unsigned char* buffer, std::streamsize n) -> std::streamsize {
            my_input->read(reinterpret_cast<char*>(buffer), n); 
            const auto my_read = my_input->gcount();
            if (my_read < n) {
                if (!(my_input->eof())) {
                    throw std::runtime_error("failed to finish reading the input stream");
                }
            }
            return my_read;
        });
    }

private:
    Pointer_ my_input;
};

}

#endif
