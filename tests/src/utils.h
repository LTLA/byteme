#ifndef UTILS_H 
#define UTILS_H 

#include <vector>
#include <random>
#include <cstddef>
#include <type_traits>
#include <fstream>

#include "byteme/Reader.hpp"
#include "byteme/Writer.hpp"

inline std::vector<unsigned char> simulate_bytes(std::size_t num, unsigned long long seed) {
    std::mt19937_64 rng(seed);
    std::vector<unsigned char> output(num);
    constexpr auto limit = std::numeric_limits<unsigned char>::max();
    for (auto& o : output) {
        o = rng() % limit;
    }
    return output;
}

inline std::vector<unsigned char> full_read(byteme::Reader& reader, std::size_t chunk) {
    std::vector<unsigned char> output;
    std::vector<unsigned char> buffer(chunk);
    while (1) {
        auto nread = reader.read(buffer.data(), chunk);
        output.insert(output.end(), buffer.data(), buffer.data() + nread);
        if (nread < chunk) {
            break;
        }
    }
    return output;
}

inline std::vector<unsigned char> full_read(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        throw std::runtime_error("failed to read '" + path + "'");
    }

    std::vector<char> tmp(std::istreambuf_iterator<char>(in), {});
    auto ptr = reinterpret_cast<unsigned char*>(tmp.data());
    return std::vector<unsigned char>(ptr, ptr + tmp.size());
}

inline std::vector<unsigned char> exact_read(byteme::Reader& reader, std::size_t chunk) {
    std::vector<unsigned char> output;
    std::vector<unsigned char> buffer(chunk);
    while (1) {
        auto nread = reader.read(buffer.data(), chunk);
        output.insert(output.end(), buffer.data(), buffer.data() + nread);
        if (nread < chunk) {
            EXPECT_EQ(nread, 0);
            break;
        }
    }
    return output;
}

inline void full_dump(byteme::Writer& writer, const std::vector<unsigned char>& contents, std::size_t chunk_size) {
    const std::size_t len = contents.size();
    for (std::size_t x = 0; x < len; x += chunk_size) {
        writer.write(contents.data() + x, std::min(chunk_size, len - x));
    }
    writer.finish();
}

#endif
