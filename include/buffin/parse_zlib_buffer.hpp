#ifndef BUFFIN_PARSE_ZLIB_BUFFER_HPP
#define BUFFIN_PARSE_ZLIB_BUFFER_HPP

#include "zlib.h"
#include <stdexcept>
#include <vector>

namespace buffin {

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
        int ret;
        if (mode == 0) { // DEFLATE
            ret = inflateInit2(&strm, -MAX_WBITS); 
        } else if (mode == 1) { // Zlib
            ret = inflateInit2(&strm, MAX_WBITS); 
        } else if (mode == 2) { // Gzip
            ret = inflateInit2(&strm, 16+MAX_WBITS); 
        } else if (mode == 3) { // Gzip/Zlib auto-detected
            ret = inflateInit2(&strm, 32+MAX_WBITS); 
        } 

        if (ret != Z_OK) {
            throw 1;
        }
    }

    ~ZStream() {
        (void)inflateEnd(&strm);
        return;
    }

    // Delete the remaining constructors.
    ZStream(const ZStream&) = delete;
    ZStream(ZStream&&) = delete;
    ZStream& operator=(const ZStream&) = delete;
    ZStream& operator=(ZStream&&) = delete;

    z_stream strm;
};
/**
 * @endcond
 */

/**
 * @tparam Parser A parser class with an `add()` method.
 *
 * @param buffer Pointer to an array containing the compressed data.
 * The lack of `const`-ness is only a consequence of the C interface - the contents of the buffer do not seem to be modified.
 * @param len Length of the `buffer` array.
 * @param parser Instance of the parser class.
 * @param mode Compression of the stream - DEFLATE (0), Zlib (1) or Gzip (2).
 * Default of 3 will auto-detect between Zlib and Gzip based on the headers.
 * @param bufsize Size of the buffer to use for reading.
 *
 * @return The buffer's contents are iteratively decompressed into a buffer that is passed to `Parser::add()` until completion.
 */
template<class Parser>
void parse_zlib_buffer(unsigned char* buffer, size_t len, Parser& parser, int mode = 3, size_t buffer_size = 65536) {
    // Stolen from 'inf()' at http://www.zlib.net/zpipe.c,
    // with some shuffling of code to make it a bit more C++-like.
    std::vector<unsigned char> output(buffer_size);

    ZStream zstr(mode);
    zstr.strm.avail_in = len;
    zstr.strm.next_in = buffer;

    while (1) {
        zstr.strm.avail_out = buffer_size;
        zstr.strm.next_out = output.data();
        int ret = inflate(&(zstr.strm), Z_NO_FLUSH);

        switch (ret) {
            case Z_STREAM_ERROR:
            case Z_NEED_DICT:
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                throw std::runtime_error("zlib error");
        }

        size_t current_stored = buffer_size - zstr.strm.avail_out;
        parser.add(output.data(), current_stored);
        
        if (ret == Z_STREAM_END) {
            break;
        }
    } 

    return;
}

}

#endif
