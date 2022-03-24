#ifndef BUFFIN_PARSE_INPUT_STREAM_HPP
#define BUFFIN_PARSE_INPUT_STREAM_HPP

#include <istream>
#include <vector>
#include <stdexcept>

/**
 * @file parse_input_stream.hpp
 *
 * @brief Parse an input stream.
 */

namespace buffin {

/**
 * @tparam Parser A parser class with an `add()` method.
 *
 * @param input An input stream.
 * @param parser Instance of the parser class.
 * @param buffer_size Size of the buffer to use for reading.
 *
 * @return The stream's contents are iteratively read into a buffer that is passed to `Parser::add()` until completion.
 */
template<class Parser>
void parse_input_stream(std::istream& input, Parser& parser, size_t buffer_size = 65536) {
    std::vector<char> buffer (buffer_size);

    while (1) {
        input.read(buffer.data(), buffer_size);
        size_t read = input.gcount();

        if (read < buffer_size) {
            if (input.eof()) {
                parser.add(buffer.data(), read);
                break;
            } else {
                throw std::runtime_error("failed to finish reading the input stream");
            }
        } else {
            parser.add(buffer.data(), buffer_size);
        }
    }

    return;
}

}

#endif
