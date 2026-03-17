#include <gtest/gtest.h>

#include "byteme/SelfClosingGzFile.hpp"
#include "byteme/GzipFileWriter.hpp"

#include "zlib.h"

#include "temp_file_path.h"
#include "utils.h"

#include <fstream>
#include <cstddef>

class GzipFileWriterTest : public ::testing::TestWithParam<std::tuple<int, int, bool> > {
protected:
    auto dump_file(const std::vector<unsigned char>& contents, std::size_t chunk_size, bool with_finish) {
        auto path = temp_file_path("text");
        byteme::GzipFileWriter writer(path.c_str(), [&]{
            byteme::GzipFileWriterOptions opt;
            opt.gzbuffer_size = 1024; // setting it just to get some test coverage of the gzbuffer_size setter.
            return opt;
        }());

        full_dump(writer, contents, chunk_size, with_finish);
        return path;
    }

    static std::vector<unsigned char> zcat(const std::string& path) { 
        auto handle = gzopen(path.c_str(), "r");
        std::vector<unsigned char> everything(10000); // some large number.
        auto read = gzread(handle, everything.data(), everything.size());
        everything.resize(read);
        return everything;
    }
};

TEST_P(GzipFileWriterTest, Basic) {
    auto param = GetParam();
    auto nbytes = std::get<0>(param);
    auto chunk_size = std::get<1>(param);
    auto with_finish = std::get<2>(param);

    auto contents = simulate_bytes(nbytes, /* seed = */ nbytes * 100 + chunk_size + int(with_finish) * 5);
    auto path = dump_file(contents, chunk_size, with_finish);
    auto roundtrip = zcat(path);
    EXPECT_EQ(roundtrip, contents);
}

INSTANTIATE_TEST_SUITE_P(
    GzipFileWriter,
    GzipFileWriterTest,
    ::testing::Combine(
        ::testing::Values(64, 200, 512, 10000), // Number of simulated bytes 
        ::testing::Values(25, 32, 64, 125), // Chunk size, some of which are factors of, less than or greater than the simulated bytes.
        ::testing::Values(true, false) // whether to call finish explicitly.
    )
);

TEST_F(GzipFileWriterTest, Empty) {
    std::vector<unsigned char> contents; 
    auto path = dump_file(contents, 20, true); 
    auto roundtrip = zcat(path);
    EXPECT_EQ(roundtrip, contents);
}

TEST_F(GzipFileWriterTest, ZeroByteWrites) {
    auto contents = simulate_bytes(321, /* seed = */ 888);
    auto path = temp_file_path("text");
    byteme::GzipFileWriter writer(path.c_str(), {});

    full_dump_with_zeros(writer, contents, 20);
    auto roundtrip = zcat(path);
    EXPECT_EQ(roundtrip, contents);
}
