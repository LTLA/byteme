#include <gtest/gtest.h>

#include "temp_file_path.h"
#include "utils.h"

#include "byteme/OstreamWriter.hpp"

#include <fstream>

class OstreamWriterTest : public ::testing::TestWithParam<std::tuple<int, int, bool> > {
protected:    
    auto dump_file(const std::vector<unsigned char>& contents, std::size_t chunk_size, bool with_finish) {
        auto path = temp_file_path("text");
        byteme::OstreamWriter writer(std::make_unique<std::ofstream>(path));
        full_dump(writer, contents, chunk_size, with_finish);
        return path;
    }
};

TEST_P(OstreamWriterTest, Basic) {
    auto params = GetParam();
    const auto nbytes = std::get<0>(params);
    const auto chunk = std::get<1>(params);
    const auto with_finish = std::get<2>(params);

    auto contents = simulate_bytes(nbytes, /* seed = */ nbytes + chunk + 10 * int(with_finish));
    auto path = dump_file(contents, chunk, with_finish);
    auto roundtrip = full_read(path);
    EXPECT_EQ(roundtrip, contents);
}

INSTANTIATE_TEST_SUITE_P(
    OstreamWriter,
    OstreamWriterTest,
    ::testing::Combine(
        ::testing::Values(64, 200, 512, 1000), // Number of simulated bytes 
        ::testing::Values(25, 32, 64, 125), // Chunk size, some of which are factors, less than or greater than the simulated bytes.
        ::testing::Values(true, false) // Whether to call finish() explicitly.
    )
);

TEST_F(OstreamWriterTest, Empty) {
    std::vector<unsigned char> contents;
    auto path = dump_file(contents, 10, true);
    auto roundtrip = full_read(path);
    EXPECT_EQ(roundtrip, contents);
}
