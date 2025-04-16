#ifndef BYTEME_MAGIC_NUMBERS_HPP
#define BYTEME_MAGIC_NUMBERS_HPP

#include <cstddef>

namespace byteme {

inline bool is_zlib(const unsigned char* values, std::size_t n) {
    if (n >= 2 && values[0] == 0x78) {
        auto level = values[1];
        // From https://en.wikipedia.org/wiki/List_of_file_signatures
        if (level == 0x01 || level == 0x5e || level == 0x9c || level == 0xda || level == 0x20 || level == 0x7d || level == 0xbb || level == 0xf9) {
            return true;
        }
    }
    return false;
}

inline bool is_gzip(const unsigned char* values, std::size_t n) {
    // ommitting the 0x08 requirement as Gzip could be used with non-DEFLATE algorithms.
    return (n >= 2 && values[0] == 0x1f && values[1] == 0x8b);
}

}

#endif
