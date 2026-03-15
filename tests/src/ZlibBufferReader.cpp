#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "temp_file_path.h"
#include "utils.h"

#include "byteme/ZlibBufferReader.hpp"
#include "zlib.h"

#include <cstdint>
#include <vector>

class ZlibBufferReaderTest : public ::testing::TestWithParam<std::tuple<int, int, int> > {
protected:
    std::vector<unsigned char> dump_file(const std::vector<unsigned char>& contents) {
        auto gzname = temp_file_path("zlib");
        gzFile ohandle = gzopen(gzname.c_str(), "w");
        gzwrite(ohandle, reinterpret_cast<const char*>(contents.data()), contents.size());
        gzclose(ohandle);
        return full_read(gzname);
    }
};

TEST_P(ZlibBufferReaderTest, Basic) {
    auto config = GetParam();
    auto nbytes = std::get<0>(config);
    auto buffer_size = std::get<1>(config);
    auto chunk_size = std::get<2>(config);

    auto contents = simulate_bytes(nbytes, /* seed = */ nbytes + buffer_size + chunk_size);
    auto gzcontents = dump_file(contents);

    byteme::ZlibBufferReader reader(gzcontents.data(), gzcontents.size(), [&]{
        byteme::ZlibBufferReaderOptions zopt;
        zopt.buffer_size = buffer_size;
        return zopt;
    }());

    auto lines = full_read(reader, chunk_size);
    EXPECT_EQ(lines, contents);

    // Trying in dedicated Gzip mode.
    {
        byteme::ZlibBufferReader reader(gzcontents.data(), gzcontents.size(), [&]{
            byteme::ZlibBufferReaderOptions zopt;
            zopt.mode = byteme::ZlibCompressionMode::GZIP;
            zopt.buffer_size = buffer_size;
            return zopt;
        }());

        auto lines = full_read(reader, chunk_size);
        EXPECT_EQ(lines, contents);
    }
}

INSTANTIATE_TEST_SUITE_P(
    ZlibBufferReader,
    ZlibBufferReaderTest,
    ::testing::Combine(
        ::testing::Values(64, 200, 512, 1000), // Number of simulated bytes 
        ::testing::Values(50, 64, 128, 250), // Buffer size, some of which are factors of, less than or greater than the simulated bytes.
        ::testing::Values(25, 32, 64, 125)  // Chunk size, some of which are factors of, less than, or greater than the simulated bytes.
    )
);

TEST_F(ZlibBufferReaderTest, Empty) {
    std::vector<unsigned char> contents;
    auto gzcontents = dump_file(contents);

    byteme::ZlibBufferReader reader(gzcontents.data(), gzcontents.size(), {});
    auto lines = full_read(reader, 123);
    EXPECT_EQ(lines, contents);
}

TEST_F(ZlibBufferReaderTest, Exact) {
    auto contents = simulate_bytes(100, /* seed = */ 123123);
    auto gzcontents = dump_file(contents);

    byteme::ZlibBufferReader reader(gzcontents.data(), gzcontents.size(), {});
    auto lines = exact_read(reader, 25);
    EXPECT_EQ(lines, contents);
}

TEST_F(ZlibBufferReaderTest, Reread) {
    // Here, we check that we correctly refill the input buffer for Zlib.
    // This is necessary as the input buffer size is an unsigned int, not a size_t;
    // so we need to feed it in a little bit at a time if we've got a very large input buffer.
    auto contents = simulate_bytes(1000, /* seed = */ 456789);
    auto compressed = dump_file(contents);
    EXPECT_TRUE(compressed.size() > 255); // enough to trigger a reread for u8, see comments below.

    byteme::ZlibBufferReader reader(compressed.data(), compressed.size(), {});
    std::vector<unsigned char> roundtrip(contents.size());

    // That said, we don't actually want to create such a large input buffer for testing.
    // So we just pretend that an unsigned int is a u8 to check that the refill is done properly.
    reader.template read0<std::uint8_t>(roundtrip.data(), roundtrip.size());
    EXPECT_EQ(contents, roundtrip);
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
