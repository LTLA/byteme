#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "read_lines.h"
#include "temp_file_path.h"

#include "byteme/ZlibBufferReader.hpp"
#include "zlib.h"

#include <fstream>
#include <cstdint>
#include <vector>

class ZlibBufferReaderTest : public ::testing::TestWithParam<std::tuple<int, int> > {
protected:
    std::vector<unsigned char> dump_file(const std::vector<std::string>& contents) {
        std::string stuff;
        for (auto c : contents) {
            stuff += c;
            stuff += '\n';
        }

        auto gzname = temp_file_path("zlib");
        gzFile ohandle = gzopen(gzname.c_str(), "w");
        gzwrite(ohandle, stuff.c_str(), stuff.size());
        gzclose(ohandle);

        std::ifstream in(gzname, std::ios::binary);
        if (!in) {
            throw std::runtime_error("failed to read '" + gzname + "'");
        }
        return std::vector<unsigned char>(std::istreambuf_iterator<char>(in), {});
    }
};

TEST_P(ZlibBufferReaderTest, Basic) {
    std::vector<std::string> contents { "asdasdasd", "sd738", "93879sdjfsjdf", "caysctgatctv", "oirtueorpr2312", "09798&a*&^&c", "((&9kkjnjsnakasd" };
    auto gzcontents = dump_file(contents);

    auto config = GetParam();
    auto chunk_size = std::get<0>(config);
    auto buffer_size = std::get<1>(config);

    byteme::ZlibBufferReader reader(gzcontents.data(), gzcontents.size(), [&]{
        byteme::ZlibBufferReaderOptions zopt;
        zopt.buffer_size = buffer_size;
        return zopt;
    }());

    auto lines = read_lines(reader, chunk_size);
    EXPECT_EQ(lines, contents);

    // Trying in dedicated Gzip mode.
    {
        byteme::ZlibBufferReader reader(gzcontents.data(), gzcontents.size(), [&]{
            byteme::ZlibBufferReaderOptions zopt;
            zopt.mode = byteme::ZlibCompressionMode::GZIP;
            zopt.buffer_size = buffer_size;
            return zopt;
        }());

        auto lines = read_lines(reader, chunk_size);
        EXPECT_EQ(lines, contents);
    }
}

TEST_P(ZlibBufferReaderTest, Empty) {
    std::vector<std::string> contents;
    auto gzcontents = dump_file(contents);

    auto config = GetParam();
    auto chunk_size = std::get<0>(config);
    auto buffer_size = std::get<1>(config);

    byteme::ZlibBufferReader reader(gzcontents.data(), gzcontents.size(), [&]{
        byteme::ZlibBufferReaderOptions zopt;
        zopt.buffer_size = buffer_size;
        return zopt;
    }());

    auto lines = read_lines(reader, chunk_size);
    EXPECT_EQ(lines, contents);
}

TEST_P(ZlibBufferReaderTest, Exact) {
    auto config = GetParam();
    auto chunk_size = std::get<0>(config);
    auto buffer_size = std::get<1>(config);

    std::vector<std::string> contents;
    for (int i = 0; i < 10; ++i) {
        contents.emplace_back(chunk_size - 1, char(i + 'a'));
    }
    auto gzcontents = dump_file(contents);

    byteme::ZlibBufferReader reader(gzcontents.data(), gzcontents.size(), [&]{
        byteme::ZlibBufferReaderOptions zopt;
        zopt.buffer_size = buffer_size;
        return zopt;
    }());

    auto lines = read_lines(reader, chunk_size);
    EXPECT_EQ(lines, contents);
}

INSTANTIATE_TEST_SUITE_P(
    ZlibBufferReader,
    ZlibBufferReaderTest,
    ::testing::Combine(
        ::testing::Values(10, 20, 50, 100), // chunk size,
        ::testing::Values(10, 25, 50, 125)  // buffer size
    )
);

TEST(ZlibBufferReader, Reread) {
    // Here, we check that we correctly refill the input buffer for Zlib.
    // This is necessary as the input buffer size is an unsigned int, not a size_t;
    // so we need to feed it in a little bit at a time if we've got a very large input buffer.
    std::vector<unsigned char> payload;
    std::mt19937_64 rng(69);
    for (int i = 0; i < 1000; ++i) {
        payload.push_back(rng() % 256);
    }

    auto gzname = temp_file_path("zlib");
    gzFile ohandle = gzopen(gzname.c_str(), "w");
    gzwrite(ohandle, payload.data(), payload.size());
    gzclose(ohandle);

    std::ifstream in(gzname, std::ios::binary);
    if (!in) {
        throw std::runtime_error("failed to read '" + gzname + "'");
    }
    std::vector<unsigned char> compressed(std::istreambuf_iterator<char>(in), {});
    EXPECT_TRUE(compressed.size() > 255); // enough to trigger a reread for u8, see comments below.

    byteme::ZlibBufferReader reader(compressed.data(), compressed.size(), {});
    std::vector<unsigned char> roundtrip(payload.size());

    // That said, we don't actually want to create such a large input buffer for testing.
    // So we just pretend that an unsigned int is a u8 to check that the refill is done properly.
    reader.template read0<std::uint8_t>(roundtrip.data(), roundtrip.size());
    EXPECT_EQ(payload, roundtrip);
}

TEST(ZlibBufferReader, OtherModes) { // We'll do some more thorough tests in ZlibBufferWriter.
    byteme::ZlibBufferReader reader1(NULL, 0, [&]{
        byteme::ZlibBufferReaderOptions zopt;
        zopt.mode = byteme::ZlibCompressionMode::DEFLATE;
        return zopt;
    }());

    byteme::ZlibBufferReader reader2(NULL, 0, [&]{
        byteme::ZlibBufferReaderOptions zopt;
        zopt.mode = byteme::ZlibCompressionMode::ZLIB;
        return zopt;
    }());
}
