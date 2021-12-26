#ifndef BUFFIN_PARSE_GZIP_FILE_HPP
#define BUFFIN_PARSE_GZIP_FILE_HPP

#include "zlib.h"
#include <stdexcept>
#include <vector>
#include <string>

/**
 * @file parse_gzip_file.hpp
 *
 * @brief Parse a Gzip-compressed file.
 */

namespace buffin {

/**
 * @cond
 */
struct GZFile {
    GZFile(const char* path) : handle(gzopen(path, "rb")) {
        if (!handle) {
            throw std::runtime_error("failed to open file at '" + std::string(path) + "'");
        }
        return;
    }

    ~GZFile() {
        gzclose(handle);
        return;
    }

    // Delete the remaining constructors.
    GZFile(const GZFile&) = delete;
    GZFile(GZFile&&) = delete;
    GZFile& operator=(const GZFile&) = delete;
    GZFile& operator=(GZFile&&) = delete;

    gzFile handle;
};
/**
 * @endcond
 */

/**
 * @tparam Parser A parser class with an `add()` method.
 *
 * @param path Path to the file.
 * @param parser Instance of the parser class.
 * @param bufsize Size of the buffer to use for reading.
 *
 * @return The file's contents are iteratively decompressed into a buffer that is passed to `Parser::add()` until completion.
 */
template<class Parser>
void parse_gzip_file(const char* path, Parser& parser, size_t buffer_size = 65536) { 
    GZFile gz(path);
    std::vector<unsigned char> buffer(buffer_size); 

    size_t leftovers = 0;
    while (1) {
        size_t read = gzread(gz.handle, buffer.data(), buffer_size);
        if (read == 0) {
            if (!gzeof(gz.handle)) { 
                int dummy;
                throw std::runtime_error(gzerror(gz.handle, &dummy));
            }
            break;
        } else {
            parser.add(buffer.data(), read);
        }
    }

    return;
}

}

#endif
