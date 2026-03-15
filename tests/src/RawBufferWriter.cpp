#include <gtest/gtest.h>

#include "byteme/RawBufferWriter.hpp"

#include "utils.h"

#include <vector>
#include <cstddef>
#include <string>

class RawBufferWriterTest : public ::testing::TestWithParam<std::tuple<int, int> > {
protected:
    std::vector<unsigned char> roundtrip(const std::vector<unsigned char>& contents, std::size_t chunk_size) {
        byteme::RawBufferWriter writer({});
        full_dump(writer, contents, chunk_size);
        return writer.get_output();
    }
};

TEST_P(RawBufferWriterTest, Basic) {
    auto param = GetParam();
    auto nbytes = std::get<0>(param);
    auto chunk_size = std::get<1>(param);

    auto contents = simulate_bytes(nbytes, /* seed = */ nbytes + 200 * chunk_size);
    auto observed = roundtrip(contents, chunk_size);
    EXPECT_EQ(observed, contents);
}

INSTANTIATE_TEST_SUITE_P(
    RawBufferWriter,
    RawBufferWriterTest,
    ::testing::Combine(
        ::testing::Values(64, 200, 512, 10000), // Number of simulated bytes 
        ::testing::Values(25, 32, 64, 125)  // Chunk size, some of which are factors of, less than or greater than the simulated bytes.
    )
);

TEST_F(RawBufferWriterTest, Empty) {
    std::vector<unsigned char> contents;
    auto observed = roundtrip(contents, 10);
    EXPECT_EQ(observed, contents);
}

TEST_F(RawBufferWriterTest, ArrayCheck) {
    std::string expected("Gloria in excelsis deo");

    byteme::RawBufferWriter writer({});
    writer.write(expected.c_str());
    writer.finish();

    const auto& stuff = writer.get_output();
    auto ptr = reinterpret_cast<const char*>(stuff.data());
    std::string extracted(ptr, ptr + stuff.size());

    EXPECT_EQ(extracted, expected);
}
