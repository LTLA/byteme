#include <gtest/gtest.h>

#include "byteme/IstreamReader.hpp"

#include "temp_file_path.h"
#include "utils.h"

#include <fstream>

class IstreamReaderTest : public ::testing::TestWithParam<std::tuple<int, int> > {
protected:
    auto dump_file(const std::vector<unsigned char>& contents) {
        auto path = temp_file_path("text");
        std::ofstream output(path);
        output.write(reinterpret_cast<const char*>(contents.data()), contents.size());
        output.close();
        return path;
    }
};

TEST_P(IstreamReaderTest, Basic) {
    auto params = GetParam();
    const auto nbytes = std::get<0>(params);
    const auto chunk = std::get<1>(params);

    auto contents = simulate_bytes(nbytes, /* seed = */ nbytes + chunk * 11);
    auto path = dump_file(contents);
    auto is = std::make_unique<std::ifstream>(path);

    byteme::IstreamReader reader(std::move(is));
    auto roundtrip = full_read(reader, chunk);
    EXPECT_EQ(roundtrip, contents);
}

INSTANTIATE_TEST_SUITE_P(
    IstreamReader,
    IstreamReaderTest,
    ::testing::Combine(
        ::testing::Values(64, 200, 512, 1000), // Number of simulated bytes 
        ::testing::Values(25, 32, 64, 125)  // Chunk size, some of which are factors of, less than or greater than the simulated bytes.
    )
);

TEST_F(IstreamReaderTest, Exact) {
    auto contents = simulate_bytes(100, /* seed = */ 1234);
    auto path = dump_file(contents);
    auto is = std::make_unique<std::ifstream>(path);

    byteme::IstreamReader reader(std::move(is));
    auto lines = exact_read(reader, 50);
    EXPECT_EQ(lines, contents);
}

TEST_F(IstreamReaderTest, Empty) {
    std::vector<unsigned char> contents;
    auto path = dump_file(contents);
    auto is = std::make_unique<std::ifstream>(path);

    byteme::IstreamReader reader(std::move(is));
    auto lines = full_read(reader, 22);
    EXPECT_EQ(lines, contents);
}
