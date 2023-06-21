#include <gtest/gtest.h>

#include "read_lines.h"
#include "byteme/RawBufferReader.hpp"
#include "byteme/SomeBufferReader.hpp"

class RawBufferReaderTest : public ::testing::Test {
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

TEST_F(RawBufferReaderTest, Basic) {
    std::vector<std::string> contents { "asdasdasd", "sd738", "93879sdjfsjdf", "caysctgatctv", "oirtueorpr2312", "09798&A*&^&c", "((&9KKJNJSNAKASd" };
    auto concat = dump_buffer(contents);

    byteme::RawBufferReader reader(concat.c_str(), concat.size());
    auto lines = read_lines(reader);
    EXPECT_EQ(lines, contents);
}

TEST_F(RawBufferReaderTest, Empty) {
    std::vector<std::string> contents { "asdasdasd", "", "", "caysctgatctv", "", "", "((&9KKJNJSNAKASd", "" };
    auto concat = dump_buffer(contents);

    byteme::RawBufferReader reader(concat.c_str(), concat.size());
    auto lines = read_lines(reader);
    EXPECT_EQ(lines, contents);
}

TEST_F(RawBufferReaderTest, TooLong) {
    std::vector<std::string> contents { "asdasdasd", "asdaisdaioufhiuvhdsiug sifyw983r7w9fsoiufhsiud nse98 98eye9s8fy siufhsu caysctgatctv", "((&9KKJNJSNAKASd" };
    auto concat = dump_buffer(contents);

    byteme::RawBufferReader reader(concat.c_str(), concat.size());
    auto lines = read_lines(reader);
    EXPECT_EQ(lines, contents);
}

TEST_F(RawBufferReaderTest, SomeBufferWorks) {
    std::vector<std::string> contents { "asdasdasd", "sd738", "93879sdjfsjdf", "caysctgatctv", "oirtueorpr2312", "09798&A*&^&c", "((&9KKJNJSNAKASd" };
    auto concat = dump_buffer(contents);

    byteme::SomeBufferReader reader(concat.c_str(), concat.size());
    auto lines = read_lines(reader);
    EXPECT_EQ(lines, contents);
}
