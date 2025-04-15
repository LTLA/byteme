#ifndef BYTEME_SOME_FILE_READER_HPP
#define BYTEME_SOME_FILE_READER_HPP

#include <memory>
#include <cstdio>
#include <cstddef>

#include "Reader.hpp"
#include "RawFileReader.hpp"
#include "GzipFileReader.hpp"
#include "magic_numbers.hpp"

/**
 * @file SomeFileReader.hpp
 *
 * @brief Read a possibly-Gzipped file.
 */

namespace byteme {

/**
 * @brief Options for the `SomeFileReader` constructor.
 */
struct SomeFileReaderOptions {
    /**
     * Size of the buffer to use when reading from disk.
     * Larger values usually reduce computational time at the cost of increased memory usage.
     */
    std::size_t buffer_size = 65536;
};

/**
 * @brief Read a file that may or may not be Gzipped.
 *
 * This class will automatically detect whether `path` refers to a text file or a Gzip-compressed file, based on its initial magic numbers.
 * After that, it will dispatch appropriately to `RawFileReader` or `GzipFileReader` respectively.
 */
class SomeFileReader final : public Reader {
public:
    /**
     * @param path Path to the file.
     * @param options Further options.
     */
    SomeFileReader(const char* path, const SomeFileReaderOptions& options) { 
        unsigned char header[3];
        std::size_t read;
        {
            SelfClosingFILE file(path, "rb");
            read = std::fread(header, sizeof(unsigned char), 3, file.handle);
        }

        if (is_gzip(header, read)) {
            GzipFileReaderOptions gopt;
            gopt.buffer_size = options.buffer_size;
            my_source.reset(new GzipFileReader(path, gopt));
        } else {
            RawFileReaderOptions ropt;
            ropt.buffer_size = options.buffer_size;
            my_source.reset(new RawFileReader(path, ropt));
        }
    }

public:
    bool load() {
        return my_source->load();
    }

    const unsigned char* buffer() const {
        return my_source->buffer();
    }

    std::size_t available() const {
        return my_source->available();
    }

private:
    std::unique_ptr<Reader> my_source;
};

}

#endif
