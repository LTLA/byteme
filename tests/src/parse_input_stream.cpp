#include <gtest/gtest.h>

#include "LineReader.h"
#include "buffin/parse_input_stream.hpp"

class ParseInputStreamTest : public ::testing::TestWithParam<int> {
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

TEST_P(ParseInputStreamTest, Basic) {
    std::vector<std::string> contents { "asdasdasd", "sd738", "93879sdjfsjdf", "caysctgatctv", "oirtueorpr2312", "09798&A*&^&c", "((&9KKJNJSNAKASd" };
    std::istringstream is(dump_buffer(contents));
    LineReader reader;
    buffin::parse_input_stream(is, reader, GetParam());
    EXPECT_EQ(reader.lines, contents);
}

TEST_P(ParseInputStreamTest, Empty) {
    std::vector<std::string> contents { "asdasdasd", "", "", "caysctgatctv", "", "", "((&9KKJNJSNAKASd", "" };
    std::istringstream is(dump_buffer(contents));
    LineReader reader;
    buffin::parse_input_stream(is, reader, GetParam());
    EXPECT_EQ(reader.lines, contents);
}

TEST_P(ParseInputStreamTest, TooLong) {
    std::vector<std::string> contents { "asdasdasd", "asdaisdaioufhiuvhdsiug sifyw983r7w9fsoiufhsiud nse98 98eye9s8fy siufhsu caysctgatctv", "((&9KKJNJSNAKASd" };
    std::istringstream is(dump_buffer(contents));
    LineReader reader;
    buffin::parse_input_stream(is, reader, GetParam());
    EXPECT_EQ(reader.lines, contents);
}

INSTANTIATE_TEST_SUITE_P(
    ParseInputStream,
    ParseInputStreamTest,
    ::testing::Values(10, 50, 100, 1000)
);
