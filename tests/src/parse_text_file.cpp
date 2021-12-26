#include <gtest/gtest.h>

#include "LineReader.h"
#include "temp_file_path.h"

#include "buffin/parse_text_file.hpp"
#include <fstream>

class ParseTextFileTest : public ::testing::TestWithParam<int> {};

TEST_P(ParseTextFileTest, Basic) {
    std::vector<std::string> contents { "asdasdasd", "sd738", "93879sdjfsjdf", "caysctgatctv", "oirtueorpr2312", "09798&A*&^&c", "((&9KKJNJSNAKASd" };

    auto path = temp_file_path("text");
    std::ofstream output(path.c_str());
    for (auto c : contents) {
        output << c << "\n";
    }
    output.close();

    LineReader reader;
    buffin::parse_text_file(path.c_str(), reader, GetParam());
    EXPECT_EQ(reader.lines, contents);
}

TEST_P(ParseTextFileTest, Empty) {
    std::vector<std::string> contents { "asdasdasd", "", "", "caysctgatctv", "", "", "((&9KKJNJSNAKASd", "" };

    auto path = temp_file_path("text");
    std::ofstream output(path.c_str());
    for (auto c : contents) {
        output << c << "\n";
    }
    output.close();

    LineReader reader;
    buffin::parse_text_file(path.c_str(), reader, GetParam());
    EXPECT_EQ(reader.lines, contents);
}

TEST_P(ParseTextFileTest, TooLong) {
    std::vector<std::string> contents { "asdasdasd", "asdaisdaioufhiuvhdsiug sifyw983r7w9fsoiufhsiud nse98 98eye9s8fy siufhsu caysctgatctv", "((&9KKJNJSNAKASd" };

    auto path = temp_file_path("text");
    std::ofstream output(path.c_str());
    for (auto c : contents) {
        output << c << "\n";
    }
    output.close();

    LineReader reader;
    buffin::parse_text_file(path.c_str(), reader, GetParam());
    EXPECT_EQ(reader.lines, contents);
}

INSTANTIATE_TEST_SUITE_P(
    ParseTextFile,
    ParseTextFileTest,
    ::testing::Values(10, 50, 100, 1000)
);
