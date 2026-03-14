#include <gtest/gtest.h>

#include "read_lines.h"
#include "byteme/IstreamReader.hpp"

class IstreamReaderTest : public ::testing::TestWithParam<int> {
protected:    
    auto dump_buffer(const std::vector<std::string>& contents) {
        std::string output;
        for (auto c : contents) {
            output += c;
            output += "\n";
        }
        return output;
    }
};

TEST_P(IstreamReaderTest, Basic) {
    std::vector<std::string> contents { "asdasdasd", "sd738", "93879sdjfsjdf", "caysctgatctv", "oirtueorpr2312", "09798&A*&^&c", "((&9KKJNJSNAKASd" };
    auto is = std::make_unique<std::istringstream>(dump_buffer(contents));

    byteme::IstreamReader reader(std::move(is));
    auto lines = read_lines(reader, GetParam());
    EXPECT_EQ(lines, contents);
}

TEST_P(IstreamReaderTest, Empty) {
    std::vector<std::string> contents;
    auto is = std::make_unique<std::istringstream>(dump_buffer(contents));

    byteme::IstreamReader reader(std::move(is));
    auto lines = read_lines(reader, GetParam());
    EXPECT_EQ(lines, contents);
}

TEST_P(IstreamReaderTest, Exact) {
    std::vector<std::string> contents;
    for (int i = 0; i < 10; ++i) {
        contents.emplace_back(GetParam() - 1, char(i + 'a')); // total size is a multiple of GetParam().
    }
    auto combined = dump_buffer(contents);
    EXPECT_EQ(combined.size() % GetParam(), 0);
    auto is = std::make_unique<std::istringstream>(std::move(combined));

    byteme::IstreamReader reader(std::move(is));
    auto lines = read_lines(reader, GetParam());
    EXPECT_EQ(lines, contents);
}

INSTANTIATE_TEST_SUITE_P(
    IstreamReader,
    IstreamReaderTest,
    ::testing::Values(10, 20, 50, 100)
);
