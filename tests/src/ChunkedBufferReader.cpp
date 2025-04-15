#include <gtest/gtest.h>

#include "read_lines.h"
#include "byteme/ChunkedBufferReader.hpp"
#include "byteme/SomeBufferReader.hpp"

class ChunkedBufferReaderTest : public ::testing::Test {
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

TEST_F(ChunkedBufferReaderTest, Basic) {
    std::vector<std::string> contents { "asdasdasd", "sd738", "93879sdjfsjdf", "caysctgatctv", "oirtueorpr2312", "09798&A*&^&c", "((&9KKJNJSNAKASd" };
    auto concat = dump_buffer(contents);

    byteme::ChunkedBufferReader reader(reinterpret_cast<const unsigned char*>(concat.c_str()), concat.size(), 10);
    auto lines = read_lines(reader);
    EXPECT_EQ(lines, contents);
}

TEST_F(ChunkedBufferReaderTest, Empty) {
    std::vector<std::string> contents { "asdasdasd", "", "", "caysctgatctv", "", "", "((&9KKJNJSNAKASd", "" };
    auto concat = dump_buffer(contents);

    byteme::ChunkedBufferReader reader(reinterpret_cast<const unsigned char*>(concat.c_str()), concat.size(), 8);
    auto lines = read_lines(reader);
    EXPECT_EQ(lines, contents);
}

TEST_F(ChunkedBufferReaderTest, TooLong) {
    std::vector<std::string> contents { "asdasdasd", "asdaisdaioufhiuvhdsiug sifyw983r7w9fsoiufhsiud nse98 98eye9s8fy siufhsu caysctgatctv", "((&9KKJNJSNAKASd" };
    auto concat = dump_buffer(contents);

    byteme::ChunkedBufferReader reader(reinterpret_cast<const unsigned char*>(concat.c_str()), concat.size(), 12);
    auto lines = read_lines(reader);
    EXPECT_EQ(lines, contents);
}
