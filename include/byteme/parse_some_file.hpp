#ifndef BUFFIN_PARSE_SOME_FILE_HPP
#define BUFFIN_PARSE_SOME_FILE_HPP

#include "parse_text_file.hpp"
#include "parse_gzip_file.hpp"
#include <cstdio>

/**
 * @file parse_some_file.hpp
 *
 * @brief Parse a text or Gzipped file.
 */

namespace buffin {

/**
 * @tparam Parser A parser class with an `add()` method.
 *
 * @param path Path to the file.
 * @param parser Instance of the parser class.
 * @param buffer_size Size of the buffer to use for reading.
 *
 * @return The file's contents are iteratively read into a buffer that is passed to `Parser::add()` until completion.
 *
 * This function will automatically detect whether `path` refers to a text file or a Gzip-compressed file,
 * and dispatch appropriately to `parse_text_file()` or `parse_gzip_file()` respectively.
 */
template<class Parser>
void parse_some_file(const char* path, Parser& parser, size_t buffer_size = 65536) {
    char header[3];
    size_t read;
    {
        SelfClosingFILE file(path);
        read = std::fread(header, sizeof(char), 3, file.handle);
    }

    if (read == 3 && static_cast<unsigned char>(header[0]) == 0x1f && static_cast<unsigned char>(header[1]) == 0x8b && static_cast<unsigned char>(header[2]) == 0x08) {
        parse_gzip_file(path, parser, buffer_size);
    } else {
        parse_text_file(path, parser, buffer_size);
    }

    return;
}

}

#endif
