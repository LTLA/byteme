#ifndef BYTEME_GZIP_FILE_WRITER_HPP
#define BYTEME_GZIP_FILE_WRITER_HPP

#include <stdexcept>
#include <vector>
#include <string>
#include "Writer.hpp"

/**
 * @file GzipFileWriter.hpp
 *
 * @brief Read a Gzip-compressed file.
 */

namespace byteme {

/**
 * @brief Write uncompressed bytes to a Gzip-compressed file.
 *
 * This is basically a wrapper around Zlib's `gzFile` with correct closing and error checking.
 */
class GzipFileWriter : public Writer {
public:
    /**
     * @param path Path to the file.
     * @param compression_level Gzip compression level. 
     * @param buffer_size Size of the internal buffer. 
     */
    GzipFileWriter(const char* path, int compression_level = 6, size_t buffer_size = 65536) : gz(path, "wb"), buffer_(buffer_size) {
        if (gzsetparams(gz.handle, compression_level, Z_DEFAULT_STRATEGY) != Z_OK) {
            throw std::runtime_error("failed to set the compression parameters");
        }
    }

    /**
     * @param path Path to the file.
     * @param compression_level Gzip compression level. 
     * @param buffer_size Size of the buffer to use for reading.
     */
    GzipFileWriter(const std::string& path, int compression_level = 6, size_t buffer_size = 65536) : GzipFileWriter(path.c_str(), compression_level, buffer_size) {}

public:
    void write(const unsigned char* buffer, size_t n) {
        if (!n) {
            return;
        }

        const auto capacity = buffer_.size();
        auto ptr = buffer_.data();

        // A bunch of choices made here to minimize the number of writes.
        if (n < capacity) {
            if (used + n < capacity) {
                std::copy(buffer, buffer + n, ptr + used);
                used += n;
            } else {
                size_t to_add = capacity - used;
                std::copy(buffer, buffer + to_add, ptr + used);
                dump(ptr, capacity);
                std::copy(buffer + to_add, buffer + n, ptr);
                used = n - to_add;
            }
        } else {
            if (used) {
                dump(ptr, used);
                used = 0;
            }
            dump(buffer, n);
        }
    }

    void finish() {
        if (!gz.closed) {
            dump(buffer_.data(), used);
            gz.closed = true;
            if (gzclose(gz.handle) != Z_OK) {
                throw std::runtime_error("failed to close the Gzip-compressed file after writing");
            }
        }
    }

private:
    SelfClosingGzFile gz;
    std::vector<unsigned char> buffer_;
    size_t used = 0;

    void dump(const unsigned char* ptr, size_t len) {
        if (!len) {
            return;
        }
        size_t ok = gzwrite(gz.handle, ptr, len);
        if (ok != len) {
            throw std::runtime_error("failed to write to the Gzip-compressed file");
        }
        return;
    }
};

}

#endif
