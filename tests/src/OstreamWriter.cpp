#include <gtest/gtest.h>

#include "temp_file_path.h"
#include "utils.h"

#include "byteme/OstreamWriter.hpp"

#include <fstream>

class OstreamWriterTest : public ::testing::TestWithParam<std::tuple<int, int> > {
protected:    
    auto dump_file(const std::vector<unsigned char>& contents, std::size_t chunk_size) {
        auto path = temp_file_path("text");
        byteme::OstreamWriter writer(std::make_unique<std::ofstream>(path));
        full_dump(writer, contents, chunk_size);
        return full_read(path);
    }
};

TEST_P(OstreamWriterTest, Basic) {
    auto params = GetParam();
    const auto nbytes = std::get<0>(params);
    const auto chunk = std::get<1>(params);

    auto contents = simulate_bytes(nbytes, /* seed = */ nbytes + chunk);
    auto roundtrip = dump_file(contents, chunk);
    EXPECT_EQ(roundtrip, contents);
}

INSTANTIATE_TEST_SUITE_P(
    OstreamWriter,
    OstreamWriterTest,
    ::testing::Combine(
        ::testing::Values(64, 200, 512, 1000), // Number of simulated bytes 
        ::testing::Values(25, 32, 64, 125)  // Chunk size, some of which are factors, less than or greater than the simulated bytes.
    )
);

TEST_F(OstreamWriterTest, Empty) {
    std::vector<unsigned char> contents;
    auto roundtrip = dump_file(contents, 10);
    EXPECT_EQ(roundtrip, contents);
}
