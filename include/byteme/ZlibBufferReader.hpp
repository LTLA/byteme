#ifndef BYTEME_ZLIB_BUFFER_READER_HPP
#define BYTEME_ZLIB_BUFFER_READER_HPP

#include "zlib.h"
#include <stdexcept>
#include <vector>
#include "Reader.hpp"

/**
 * @file ZlibBufferReader.hpp
 *
 * @brief Read bytes from a Zlib-compressed buffer.
 */

namespace byteme {

/**
 * @brief Read and decompress bytes from a Zlib-compressed buffer.
 *
 * This is basically a wrapper around Zlib's inflate method, with correct closing and error checking.
 */
class ZlibBufferReader final : public Reader {
private:
    /**
     * @cond
     */
    struct ZStream {
        ZStream(int mode) {
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
            int ret = 0;
            if (mode == 0) { // DEFLATE
                ret = inflateInit2(&strm, -MAX_WBITS); 
            } else if (mode == 1) { // Zlib
                ret = inflateInit2(&strm, MAX_WBITS); 
            } else if (mode == 2) { // Gzip
                ret = inflateInit2(&strm, 16 + MAX_WBITS); 
            } else if (mode == 3) { // Gzip/Zlib auto-detected
                ret = inflateInit2(&strm, 32 + MAX_WBITS); 
            } else {
                throw std::runtime_error("mode must be 0 (DEFLATE), 1 (Zlib), 2 (Gzip) or 3 (automatic)");
            }

            if (ret != Z_OK) {
                throw 1;
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
    /**
     * @endcond
     */

public:
    /**
     * @param buffer Pointer to an array containing the compressed data.
     * @param length Length of the `buffer` array.
     * @param mode Compression of the stream - DEFLATE (0), Zlib (1) or Gzip (2).
     * Default of 3 will auto-detect between Zlib and Gzip based on the headers.
     * @param buffer_size Size of the buffer to use for reading.
     */
    ZlibBufferReader(const unsigned char* buffer, size_t length, int mode = 3, size_t buffer_size = 65536) : my_zstr(mode), my_buffer(buffer_size) {
        my_zstr.strm.avail_in = length;
        my_zstr.strm.next_in = const_cast<unsigned char*>(buffer); // cast is purely for C compatibility.
    }

public:
    bool load() {
        /* This function is stolen from the loop in 'inf()' at
         * http://www.zlib.net/zpipe.c, with some shuffling of code to make it
         * a bit more C++-like.
         */

        if (!my_okay) {
            return false;
        }

        // Not entirely sure why we need to check for this, but
        // https://zlib.net/zpipe.c does it, and so will we; because not doing
        // so seems to occasionally result in infinite loops.
        if (my_zstr.strm.avail_in == 0) {
            return false;
        }

        my_zstr.strm.avail_out = my_buffer.size();
        my_zstr.strm.next_out = my_buffer.data();
        int ret = inflate(&(my_zstr.strm), Z_NO_FLUSH);

        switch (ret) {
            case Z_STREAM_ERROR:
            case Z_NEED_DICT:
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                throw std::runtime_error("zlib error");
            case Z_STREAM_END:
                my_okay = false;
                break;
        }

        my_read = my_buffer.size() - my_zstr.strm.avail_out;
        return true;
    }

    const unsigned char* buffer() const {
        return my_buffer.data();
    }

    size_t available() const {
        return my_read;
    }

private:
    ZStream my_zstr;
    std::vector<unsigned char> my_buffer;
    size_t my_read = 0;
    bool my_okay = true;
};

}

#endif
