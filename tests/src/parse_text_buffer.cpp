#include <gtest/gtest.h>

#include "LineReader.h"
#include "buffin/parse_text_buffer.hpp"

class ParseTextBufferTest : public ::testing::TestWithParam<int> {
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

TEST_P(ParseTextBufferTest, Basic) {
    std::vector<std::string> contents { "asdasdasd", "sd738", "93879sdjfsjdf", "caysctgatctv", "oirtueorpr2312", "09798&A*&^&c", "((&9KKJNJSNAKASd" };
    auto concat = dump_buffer(contents);
    LineReader reader;
    buffin::parse_text_buffer(concat.c_str(), concat.size(), reader, GetParam());
    EXPECT_EQ(reader.lines, contents);
}

TEST_P(ParseTextBufferTest, Empty) {
    std::vector<std::string> contents { "asdasdasd", "", "", "caysctgatctv", "", "", "((&9KKJNJSNAKASd", "" };
    auto concat = dump_buffer(contents);
    LineReader reader;
    buffin::parse_text_buffer(concat.c_str(), concat.size(), reader, GetParam());
    EXPECT_EQ(reader.lines, contents);
}

TEST_P(ParseTextBufferTest, TooLong) {
    std::vector<std::string> contents { "asdasdasd", "asdaisdaioufhiuvhdsiug sifyw983r7w9fsoiufhsiud nse98 98eye9s8fy siufhsu caysctgatctv", "((&9KKJNJSNAKASd" };
    auto concat = dump_buffer(contents);
    LineReader reader;
    buffin::parse_text_buffer(concat.c_str(), concat.size(), reader, GetParam());
    EXPECT_EQ(reader.lines, contents);
}

INSTANTIATE_TEST_SUITE_P(
    ParseTextBuffer,
    ParseTextBufferTest,
    ::testing::Values(10, 50, 100, 1000)
);
