#include <gtest/gtest.h>

#include "byteme/utils.hpp"
#include "sanisizer/sanisizer.hpp"

#include "utils.h"

#include <vector>
#include <cstdint>
#include <algorithm>

class SafeReadTest : public ::testing::TestWithParam<std::tuple<int, int> > {};

TEST_P(SafeReadTest, SafeRead) {
    auto config = GetParam();
    auto nbytes = std::get<0>(config);
    auto nrequest = std::get<1>(config);
    auto contents = simulate_bytes(nbytes, /* seed = */ 69 + nbytes + 10 * nrequest);

    std::size_t used_up_to = 0;
    std::vector<unsigned char> collected(nrequest);
    auto out = byteme::safe_read<std::uint8_t>(collected.data(), collected.size(), [&](unsigned char* x, std::uint8_t len) -> std::uint8_t {
        auto remaining = sanisizer::min(len, contents.size() - used_up_to);
        std::copy_n(contents.data() + used_up_to, remaining, x);
        used_up_to += remaining;
        return remaining;
    });

    EXPECT_EQ(out, sanisizer::min(nrequest, nbytes));
    collected.resize(out);
    contents.resize(out);
    EXPECT_EQ(collected, contents);
}

INSTANTIATE_TEST_SUITE_P(
    Utils,
    SafeReadTest,
    ::testing::Combine(
        ::testing::Values(100, 255, 500, 1000), // number of simulated bytes, adding 255 to represent the u8's max.
        ::testing::Values(100, 255, 500, 1000) // number of requested bytes, sometimes smaller or larger than the number of simulated bytes.
    )
);

class SafeWriteTest : public ::testing::TestWithParam<int> {};

TEST_P(SafeWriteTest, SafeWrite) {
    auto nbytes = GetParam();
    auto contents = simulate_bytes(nbytes, /* seed = */ 42 + nbytes);

    {
        std::vector<unsigned char> collected;
        byteme::safe_write<std::uint8_t, true>(contents.data(), contents.size(), [&](const unsigned char* x, std::uint8_t len) -> void {
            collected.insert(collected.end(), x, x + len);
        });
        EXPECT_EQ(collected, contents);
    }

    {
        std::vector<unsigned char> collected;
        byteme::safe_write<std::uint8_t, false>(contents.data(), contents.size(), [&](const unsigned char* x, std::uint8_t len) -> void {
            collected.insert(collected.end(), x, x + len);
        });
        EXPECT_EQ(collected, contents);
    }
}

INSTANTIATE_TEST_SUITE_P(
    Utils,
    SafeWriteTest,
    ::testing::Values(100, 255, 500, 1000) // number of simulated bytes, adding 255 to represent the u8's max.
);
