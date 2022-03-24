#ifndef BUFFIN_PARSE_TEXT_BUFFER_HPP
#define BUFFIN_PARSE_TEXT_BUFFER_HPP

#include <algorithm>

/**
 * @file parse_text_buffer.hpp
 *
 * @brief Parse a text buffer.
 */

namespace buffin {

/**
 * @tparam B Character type for the buffer.
 * @tparam Parser A parser class with an `add()` method.
 *
 * @param buffer Pointer to an array containing the text data.
 * @param len Length of the `buffer` array.
 * @param parser Instance of the parser class.
 * @param buffer_size Size of the buffer to use for reading.
 *
 * @return The buffer's contents are iteratively parsed by `Parser::add()` until completion.
 *
 * This function enables convenient testing of the correct behavior of `Parser::add()` in the presence of chunks.
 * For usage on a real text buffer, calling `Parser:add()` directly on `buffer` and `len` should be good enough.
 */
template<typename B, class Parser>
void parse_text_buffer(const B* buffer, size_t len, Parser& parser, size_t buffer_size = 65536) {
    size_t i = 0;
    while (i < len) {
        size_t chunk = std::min(buffer_size, len - i);
        parser.add(buffer + i, chunk);
        i += chunk;
    }
    return;
}

}

#endif
