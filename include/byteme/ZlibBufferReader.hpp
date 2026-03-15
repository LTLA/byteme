#ifndef BYTEME_ZLIB_BUFFER_READER_HPP
#define BYTEME_ZLIB_BUFFER_READER_HPP

#include <stdexcept>
#include <vector>
#include <cstddef>
#include <optional>

#include "zlib.h"
#include "sanisizer/sanisizer.hpp"

#include "Reader.hpp"
#include "magic_numbers.hpp"
#include "utils.hpp"

/**
 * @file ZlibBufferReader.hpp
 *
 * @brief Read bytes from a Zlib-compressed buffer.
 */

namespace byteme {

/**
 * @brief Options for the `ZlibBufferReader` constructor.
 */
struct ZlibBufferReaderOptions {
    /**
     * Compression mode of the stream.
     * If unset, the function will auto-detect the format, choosing between ZLIB and GZIP based on the headers.
     */
    std::optional<ZlibCompressionMode> mode;

    /**
     * Size of the buffer to use when reading from disk.
     * Larger values usually reduce computational time at the cost of increased memory usage.
     */
    std::size_t buffer_size = sanisizer::cap<std::size_t>(65536);
};

/**
 * @brief Read and decompress bytes from a Zlib-compressed buffer.
 *
 * This is basically a wrapper around Zlib's inflate method, with correct closing and error checking.
 */
class ZlibBufferReader final : public Reader {
private:
    struct ZStream {
        ZStream(std::optional<ZlibCompressionMode> mode) {
            /* allocate inflate state */
            strm.zalloc = Z_NULL;
            strm.zfree = Z_NULL;
            strm.opaque = Z_NULL;
            strm.avail_in = 0;
            strm.next_in = Z_NULL;

            /* See:
             * https://stackoverflow.com/questions/1838699/how-can-i-decompress-a-gzip-stream-with-zlib
             * https://stackoverflow.com/questions/29003909/why-is-a-different-zlib-window-bits-value-required-for-extraction-compared-with
             */
            int ret;
            if (mode.has_value()) {
                switch (*mode) {
                    case ZlibCompressionMode::DEFLATE:
                        ret = inflateInit2(&strm, -MAX_WBITS); 
                        break;
                    case ZlibCompressionMode::ZLIB:
                        ret = inflateInit2(&strm, MAX_WBITS); 
                        break;
                    case ZlibCompressionMode::GZIP:
                        ret = inflateInit2(&strm, 16 + MAX_WBITS); 
                        break;
                    default:
                        throw std::runtime_error("unknown Zlib compression mode");
                }
            } else {
                // Autodetection of Gzip or Zlib. 
                ret = inflateInit2(&strm, 32 + MAX_WBITS); 
            }

            if (ret != Z_OK) {
                throw std::runtime_error(strm.msg);
            }
        }

        ~ZStream() {
            inflateEnd(&strm);
            return;
        }

    public:
        // Delete the remaining constructors.
        ZStream(const ZStream&) = delete;
        ZStream(ZStream&&) = delete;
        ZStream& operator=(const ZStream&) = delete;
        ZStream& operator=(ZStream&&) = delete;

    public:
        z_stream strm;
    };

    typedef I<decltype(I<decltype(ZStream::strm)>::avail_out)> ZoutSize;

public:
    /**
     * @param buffer Pointer to an array containing the compressed data.
     * @param length Length of the `buffer` array.
     * @param options Further options.
     */
    ZlibBufferReader(const unsigned char* buffer, std::size_t length, const ZlibBufferReaderOptions& options) : 
        my_zstr(options.mode),
        // Cap it for both allocation safety and to avoid problems from overflow of the avail_out member..
        my_buffer(sanisizer::cap<I<decltype(my_buffer.size())> >(sanisizer::cap<ZoutSize>(options.buffer_size))),
        my_input(buffer),
        my_length(length)
    {}

public:
    std::size_t read(unsigned char* buffer, std::size_t n) {
        return read0<ZoutSize>(buffer, n);
    }

    // Test function to check that the overflow protection works correctly for the input size.
    template<typename InSize>
    std::size_t read0(unsigned char* buffer, std::size_t n) {
        return safe_read<ZoutSize>(buffer, n, [this](unsigned char* buffer, ZoutSize n) -> ZoutSize {
            return this->read_internal<InSize>(buffer, n);
        });
    }

private:
    template<typename InSize_>
    ZoutSize read_internal(unsigned char* buffer, ZoutSize n) {
        auto remaining = n;

        if (my_cached_length) {
            const auto limit = std::min(remaining, my_cached_length);
            std::copy_n(my_buffer.data() + my_cached_start, limit, buffer);
            buffer += limit; 
            remaining -= limit; 
            my_cached_start += limit;
            my_cached_length -= limit;
        }

        if (remaining) {
            while (!my_finished) {
                if (my_zstr.strm.avail_in == 0) {
                    const auto to_use = sanisizer::cap<InSize_>(my_length);
                    my_zstr.strm.avail_in = to_use;
                    my_length -= to_use;
                    my_zstr.strm.next_in = const_cast<unsigned char*>(my_input); // cast is purely for C compatibility.
                    my_input += to_use;
                }

                /* This function is stolen from the loop in 'inf()' at
                 * http://www.zlib.net/zpipe.c, with some shuffling of code to make it
                 * a bit more C++-like.
                 */
                my_zstr.strm.avail_out = my_buffer.size();
                my_zstr.strm.next_out = my_buffer.data();
                auto ret = inflate(&(my_zstr.strm), Z_NO_FLUSH);

                switch (ret) {
                    case Z_STREAM_ERROR:
                    case Z_NEED_DICT:
                    case Z_DATA_ERROR:
                    case Z_MEM_ERROR:
                        throw std::runtime_error("zlib error");
                    case Z_STREAM_END:
                        my_finished = true;
                        break;
                }

                const ZoutSize nread = my_buffer.size() - my_zstr.strm.avail_out;
                if (nread < remaining) {
                    std::copy_n(my_buffer.data(), nread, buffer);
                    buffer += nread;
                    remaining -= nread;
                } else {
                    std::copy_n(my_buffer.data(), remaining, buffer);                                
                    my_cached_start = remaining;
                    my_cached_length = nread - remaining;
                    remaining = 0;
                    break;
                }
            }
        }

        return n - remaining;
    }

private:
    ZStream my_zstr;
    std::vector<unsigned char> my_buffer;
    const unsigned char* my_input;
    std::size_t my_length;
    ZoutSize my_cached_start = 0, my_cached_length = 0;
    bool my_finished = false;
};

}

#endif
