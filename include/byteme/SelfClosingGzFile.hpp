#ifndef BYTEME_SELF_CLOSING_GZFILE_HPP
#define BYTEME_SELF_CLOSING_GZFILE_HPP

#include <stdexcept>
#include <string>
#include <optional>

#include "zlib.h"

namespace byteme {

class SelfClosingGzFile {
public:
    SelfClosingGzFile(const char* path, const char* mode) : handle(gzopen(path, mode)) {
        if (!handle) {
            throw std::runtime_error("failed to open file at '" + std::string(path) + "'");
        }
        return;
    }

    ~SelfClosingGzFile() {
        if (!closed) {
            gzclose(handle);
        }
        return;
    }

public:
    SelfClosingGzFile(SelfClosingGzFile&& x) : handle(std::move(x.handle)) {
        x.closed = true;
    }

    SelfClosingGzFile& operator=(SelfClosingGzFile&& x) {
        handle = std::move(x.handle);
        x.closed = true;
        return *this;
    }

    // Delete the remaining constructors.
    SelfClosingGzFile(const SelfClosingGzFile&) = delete;
    SelfClosingGzFile& operator=(const SelfClosingGzFile&) = delete;

public:
    bool closed = false;
    gzFile handle;
};

inline void set_optional_gzbuffer_size(SelfClosingGzFile& gzfile, const std::optional<unsigned>& gzbuffer_size) {
    if (gzbuffer_size.has_value()) {
        if (gzbuffer(gzfile.handle, *gzbuffer_size)) {
            throw std::runtime_error("failed to modify the Gzip (de)compression buffer size");
        }
    }
}

}

#endif
