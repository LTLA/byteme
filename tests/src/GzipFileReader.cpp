#include <gtest/gtest.h>

#include "utils.h"
#include "temp_file_path.h"

#include "byteme/GzipFileReader.hpp"

#include "zlib.h"
#include <fstream>

class GzipFileReaderTest : public ::testing::TestWithParam<std::tuple<int, int> > {
protected:
    auto dump_file(const std::vector<unsigned char>& contents) {
        auto gzname = temp_file_path("gzip");
        gzFile ohandle = gzopen(gzname.c_str(), "w");
        gzwrite(ohandle, reinterpret_cast<const char*>(contents.data()), contents.size());
        gzclose(ohandle);
        return gzname;
    }
};

TEST_P(GzipFileReaderTest, Basic) {
    auto params = GetParam();
    const auto nbytes = std::get<0>(params);
    const auto chunk = std::get<1>(params);

    auto contents = simulate_bytes(nbytes, /* seed = */ nbytes + chunk * 13);
    auto path = dump_file(contents);

    byteme::GzipFileReader reader(path.c_str(), {});
    auto roundtrip = full_read(reader, chunk); 
    EXPECT_EQ(roundtrip, contents);
}

INSTANTIATE_TEST_SUITE_P(
    GzipFileReader,
    GzipFileReaderTest,
    ::testing::Combine(
        ::testing::Values(64, 200, 512, 1000), // Number of simulated bytes 
        ::testing::Values(25, 32, 64, 125)  // Chunk size, some of which are factors of, less than or greater than the simulated bytes.
    )
);

TEST_F(GzipFileReaderTest, Exact) {
    auto contents = simulate_bytes(100, /* seed = */ 1234);
    auto path = dump_file(contents);

    byteme::GzipFileReader reader(path.c_str(), {});
    auto lines = exact_read(reader, 25);
    EXPECT_EQ(lines, contents);
}

TEST_F(GzipFileReaderTest, Empty) {
    std::vector<unsigned char> contents;
    auto path = dump_file(contents);

    byteme::GzipFileReader reader(path.c_str(), {});
    auto lines = full_read(reader, 10);
    EXPECT_EQ(lines, contents);
}

TEST_F(GzipFileReaderTest, Moveable) {
    auto contents = simulate_bytes(192, /* seed = */ 12345);
    auto path = dump_file(contents);

    // Move constructor.
    {
        byteme::GzipFileReader reader(path.c_str(), {});
        byteme::GzipFileReader other(std::move(reader));
        auto lines = full_read(other, 13);
        EXPECT_EQ(lines, contents);
    }

    // Move assignment.
    {
        byteme::GzipFileReader reader(path.c_str(), {});
        byteme::GzipFileReader other = std::move(reader);
        auto lines = full_read(other, 22);
        EXPECT_EQ(lines, contents);
    }
}
