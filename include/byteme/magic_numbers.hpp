#ifndef BYTEME_MAGIC_NUMBERS_HPP
#define BYTEME_MAGIC_NUMBERS_HPP

#include <cstddef>

#include "SelfClosingFILE.hpp"

/**
 * @file magic_numbers.hpp
 * @brief Magic numbers for various compression algorithms.
 */

namespace byteme {

/**
 * @param[in] buffer Pointer to an array of length `n`.
 * @param n Number of bytes in `buffer`. 
 * @return Whether the buffer is Zlib-compressed, based on its magic numbers.
 */
inline bool is_zlib(const unsigned char* buffer, std::size_t n) {
    if (n >= 2 && buffer[0] == 0x78) {
        auto level = buffer[1];
        // From https://en.wikipedia.org/wiki/List_of_file_signatures
        if (level == 0x01 || level == 0x5e || level == 0x9c || level == 0xda || level == 0x20 || level == 0x7d || level == 0xbb || level == 0xf9) {
            return true;
        }
    }
    return false;
}

/**
 * @param[in] buffer Pointer to an array of length `n`.
 * @param n Number of bytes in `buffer`. 
 * @return Whether the buffer contains the bytes of a Gzipped file, based on its magic numbers.
 */
inline bool is_gzip(const unsigned char* buffer, std::size_t n) {
    // ommitting the 0x08 requirement as Gzip could be used with non-DEFLATE algorithms.
    return (n >= 2 && buffer[0] == 0x1f && buffer[1] == 0x8b);
}

/**
 * @param path Path to a file.
 * @return Whether the file is Gzipped, based on its magic numbers.
 */
inline bool is_gzip(const char* path) {
    SelfClosingFILE file(path, "rb");
    unsigned char header[3];
    auto read = std::fread(header, sizeof(unsigned char), 3, file.handle);
    return is_gzip(header, read);
}

/**
 * Compression mode for Zlib.
 * DEFLATE is the underlying compression algorithm,
 * while ZLIB and GZIP are formats that use DEFLATE internally with some extra wrappers.
 */
enum class ZlibCompressionMode : char { DEFLATE, ZLIB, GZIP };

}

#endif
