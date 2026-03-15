#include <gtest/gtest.h>

#include "read_lines.h"
#include "byteme/RawBufferReader.hpp"

class RawBufferReaderTest : public ::testing::TestWithParam<int> {
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

TEST_P(RawBufferReaderTest, Basic) {
    std::vector<std::string> contents { "asdasdasd", "sd738", "93879sdjfsjdf", "caysctgatctv", "oirtueorpr2312", "09798&A*&^&c", "((&9KKJNJSNAKASd" };
    auto concat = dump_buffer(contents);

    byteme::RawBufferReader reader(reinterpret_cast<const unsigned char*>(concat.c_str()), concat.size());
    auto lines = read_lines(reader, GetParam());
    EXPECT_EQ(lines, contents);
}

TEST_P(RawBufferReaderTest, Empty) {
    std::vector<std::string> contents;
    auto concat = dump_buffer(contents);

    byteme::RawBufferReader reader(reinterpret_cast<const unsigned char*>(concat.c_str()), concat.size());
    auto lines = read_lines(reader, GetParam());
    EXPECT_EQ(lines, contents);
}

TEST_P(RawBufferReaderTest, Exact) {
    std::vector<std::string> contents;
    for (int i = 0; i < 10; ++i) {
        contents.emplace_back(GetParam() - 1, char(i + 'a'));
    }
    auto concat = dump_buffer(contents);
    EXPECT_EQ(concat.size() % GetParam(), 0);

    byteme::RawBufferReader reader(reinterpret_cast<const unsigned char*>(concat.c_str()), concat.size());
    auto lines = read_lines(reader, GetParam());
    EXPECT_EQ(lines, contents);
}

INSTANTIATE_TEST_SUITE_P(
    RawBufferReader,
    RawBufferReaderTest,
    ::testing::Values(10, 20, 50, 100)
);
