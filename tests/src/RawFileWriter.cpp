#include <gtest/gtest.h>

#include "temp_file_path.h"
#include "utils.h"

#include "byteme/RawFileWriter.hpp"

#include <vector>
#include <cstddef>

class RawFileWriterTest : public ::testing::TestWithParam<std::tuple<int, int, bool> > {
protected:
    auto dump_file(const std::vector<unsigned char>& contents, std::size_t chunk_size, bool with_finish) {
        auto path = temp_file_path("text");
        byteme::RawFileWriter writer(path.c_str(), [&]{
            byteme::RawFileWriterOptions ropt;
            ropt.bufsiz = 1024; // setting it just to get some test coverage on the bufsize setter.
            return ropt;
        }());

        full_dump(writer, contents, chunk_size, with_finish);
        return path;
    }
};

TEST_P(RawFileWriterTest, Basic) {
    auto param = GetParam();
    auto nbytes = std::get<0>(param);
    auto chunk_size = std::get<1>(param);
    auto with_finish = std::get<2>(param);

    auto contents = simulate_bytes(nbytes, /* seed = */ nbytes + 200 * chunk_size + 100 * int(with_finish));
    auto path = dump_file(contents, chunk_size, with_finish);
    auto roundtrip = full_read(path);
    EXPECT_EQ(contents, roundtrip);
}

INSTANTIATE_TEST_SUITE_P(
    RawFileWriter,
    RawFileWriterTest,
    ::testing::Combine(
        ::testing::Values(64, 200, 512, 10000), // Number of simulated bytes.
        ::testing::Values(25, 32, 64, 125), // Chunk size, some of which are factors of, less than or greater than the simulated bytes.
        ::testing::Values(true, false) // Whether to call finish() explicitly.
    )
);

TEST_P(RawFileWriterTest, Empty) {
    std::vector<unsigned char> contents;
    auto path = dump_file(contents, 1234, true);
    auto roundtrip = full_read(path);
    EXPECT_EQ(contents, roundtrip);
}
