#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "byteme/ZlibBufferWriter.hpp"
#include "byteme/ZlibBufferReader.hpp"

#include "zlib.h"

#include <fstream>
#include <vector>
#include <cstddef>

#include "temp_file_path.h"
#include "utils.h"

class ZlibBufferWriterTest : public ::testing::TestWithParam<std::tuple<byteme::ZlibCompressionMode, int, int, int> > {
protected:
    std::vector<unsigned char> roundtrip(
        const std::vector<unsigned char>& contents,
        byteme::ZlibCompressionMode mode,
        std::size_t buffer_size,
        std::size_t chunk_size
    ) {
        byteme::ZlibBufferWriter writer([&]{
            byteme::ZlibBufferWriterOptions zopt;
            zopt.mode = mode;
            zopt.buffer_size = buffer_size;
            return zopt;
        }());

        full_dump(writer, contents, chunk_size, /* with_finish = */ true);
        const auto& compressed = writer.get_output();

        if (mode == byteme::ZlibCompressionMode::GZIP) {
            auto path = temp_file_path("temp_");
            {
                std::ofstream dump(path, std::ios::binary);
                dump.write(reinterpret_cast<const char*>(compressed.data()), compressed.size());
            }

            gzFile ohandle = gzopen(path.c_str(), "r");
            std::vector<unsigned char> holding(contents.size() + 10);
            std::size_t avail = gzread(ohandle, holding.data(), holding.size());
            gzclose(ohandle);

            EXPECT_LT(avail, holding.size());
            holding.resize(avail);
            return holding;

        } else {
            byteme::ZlibBufferReader reader(compressed.data(), compressed.size(), [&]{
                byteme::ZlibBufferReaderOptions zopt;
                zopt.mode = mode;
                return zopt;
            }());

            return full_read(reader, 100);
        }
    }
};

TEST_P(ZlibBufferWriterTest, Basic) {
    auto param = GetParam();
    auto mode = std::get<0>(param);
    auto nbytes = std::get<1>(param);
    auto buffer_size = std::get<2>(param);
    auto chunk = std::get<3>(param);

    auto contents = simulate_bytes(nbytes, /* seed = */ nbytes + buffer_size + chunk + int(mode) * 100);
    auto observed = roundtrip(contents, mode, buffer_size, chunk);
    EXPECT_EQ(observed, contents);
}

INSTANTIATE_TEST_SUITE_P(
    ZlibBufferWriter,
    ZlibBufferWriterTest,
    ::testing::Combine(
        ::testing::Values(byteme::ZlibCompressionMode::DEFLATE, byteme::ZlibCompressionMode::ZLIB, byteme::ZlibCompressionMode::GZIP), // compression mode 
        ::testing::Values(64, 200, 512, 1000), // Number of simulated bytes 
        ::testing::Values(50, 64, 128, 250), // Buffer size, some of which are factors of, less than or greater than the simulated bytes.
        ::testing::Values(25, 32, 64, 125) // Chunk size, some of which are factors of, less than or greater than the simulated bytes.
    )
);

TEST_F(ZlibBufferWriterTest, Empty) {
    std::vector<unsigned char> contents;
    auto observed = roundtrip(contents, byteme::ZlibCompressionMode::DEFLATE, 1000, 10);
    EXPECT_EQ(observed, contents);
}
