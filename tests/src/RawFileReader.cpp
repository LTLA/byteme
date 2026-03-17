#include <gtest/gtest.h>

#include "byteme/RawFileReader.hpp"

#include "temp_file_path.h"
#include "utils.h"

#include <fstream>

class RawFileReaderTest : public ::testing::TestWithParam<std::tuple<int, int> > {
protected:    
    auto dump_file(const std::vector<unsigned char>& contents) {
        auto path = temp_file_path("text");
        std::ofstream output(path);
        output.write(reinterpret_cast<const char*>(contents.data()), contents.size());
        output.close();
        return path;
    }
};

TEST_P(RawFileReaderTest, Basic) {
    auto params = GetParam();
    const auto nbytes = std::get<0>(params);
    const auto chunk = std::get<1>(params);

    auto contents = simulate_bytes(nbytes, /* seed = */ nbytes + chunk * 7);
    auto path = dump_file(contents);

    byteme::RawFileReader reader(path.c_str(), {});
    auto lines = full_read(reader, chunk);
    EXPECT_EQ(lines, contents);
}

INSTANTIATE_TEST_SUITE_P(
    RawFileReader,
    RawFileReaderTest,
    ::testing::Combine(
        ::testing::Values(64, 200, 512, 1000), // Number of simulated bytes 
        ::testing::Values(25, 32, 64, 125)  // Chunk size, some of which are factors of the simulated bytes.
    )
);

TEST_F(RawFileReaderTest, Exact) {
    auto contents = simulate_bytes(100, /* seed = */ 2468);
    auto path = dump_file(contents);

    byteme::RawFileReader reader(path.c_str(), {});
    auto lines = exact_read(reader, 10);
    EXPECT_EQ(lines, contents);
}

TEST_F(RawFileReaderTest, Empty) {
    std::vector<unsigned char> contents;
    auto path = dump_file(contents);

    byteme::RawFileReader reader(path.c_str(), {});
    auto lines = full_read(reader, 112);
    EXPECT_EQ(lines, contents);
}

TEST_F(RawFileReaderTest, ZeroByteReads) {
    auto contents = simulate_bytes(199, /* seed = */ 1234);
    auto path = dump_file(contents);

    byteme::RawFileReader reader(path.c_str(), {});
    auto lines = full_read_with_zeros(reader, 17);
    EXPECT_EQ(lines, contents);
}

TEST_F(RawFileReaderTest, Moveable) {
    auto contents = simulate_bytes(201, /* seed = */ 4321);
    auto path = dump_file(contents);

    // Move constructor.
    {
        byteme::RawFileReader reader(path.c_str(), {});
        byteme::RawFileReader other(std::move(reader));
        auto lines = full_read(other, 15);
        EXPECT_EQ(lines, contents);
    }

    // Move assignment.
    {
        byteme::RawFileReader reader(path.c_str(), {});
        byteme::RawFileReader other = std::move(reader);
        auto lines = full_read(other, 20);
        EXPECT_EQ(lines, contents);
    }
}
