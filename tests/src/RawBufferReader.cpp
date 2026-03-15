#include <gtest/gtest.h>

#include "byteme/RawBufferReader.hpp"

#include "utils.h"

class RawBufferReaderTest : public ::testing::TestWithParam<std::tuple<int, int> > {};

TEST_P(RawBufferReaderTest, Basic) {
    auto params = GetParam();
    const auto nbytes = std::get<0>(params);
    const auto chunk = std::get<1>(params);

    auto contents = simulate_bytes(nbytes, /* seed = */ nbytes + chunk * 11);

    byteme::RawBufferReader reader(contents.data(), contents.size());
    auto lines = full_read(reader, chunk);
    EXPECT_EQ(lines, contents);
}

INSTANTIATE_TEST_SUITE_P(
    RawBufferReader,
    RawBufferReaderTest,
    ::testing::Combine(
        ::testing::Values(64, 200, 512, 1000), // Number of simulated bytes 
        ::testing::Values(25, 32, 64, 125)  // Chunk size, some of which are factors of, less than or greater than the simulated bytes.
    )
);

TEST_F(RawBufferReaderTest, Exact) {
    auto contents = simulate_bytes(100, /* seed = */ 1357);

    byteme::RawBufferReader reader(contents.data(), contents.size());
    auto lines = exact_read(reader, 20);
    EXPECT_EQ(lines, contents);
}

TEST_P(RawBufferReaderTest, Empty) {
    std::vector<unsigned char> contents;

    byteme::RawBufferReader reader(contents.data(), contents.size());
    auto lines = full_read(reader, 15);
    EXPECT_EQ(lines, contents);
}
