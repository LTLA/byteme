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
    std::istringstream is(dump_buffer(contents));

    byteme::IstreamReader reader(is, GetParam());
    auto lines = read_lines(reader);
    EXPECT_EQ(lines, contents);
}

TEST_P(IstreamReaderTest, Empty) {
    std::vector<std::string> contents { "asdasdasd", "", "", "caysctgatctv", "", "", "((&9KKJNJSNAKASd", "" };
    std::istringstream is(dump_buffer(contents));

    byteme::IstreamReader reader(is, GetParam());
    auto lines = read_lines(reader);
    EXPECT_EQ(lines, contents);
}

TEST_P(IstreamReaderTest, TooLong) {
    std::vector<std::string> contents { "asdasdasd", "asdaisdaioufhiuvhdsiug sifyw983r7w9fsoiufhsiud nse98 98eye9s8fy siufhsu caysctgatctv", "((&9KKJNJSNAKASd" };
    std::istringstream is(dump_buffer(contents));

    byteme::IstreamReader reader(is, GetParam());
    auto lines = read_lines(reader);
    EXPECT_EQ(lines, contents);
}

INSTANTIATE_TEST_SUITE_P(
    IstreamReader,
    IstreamReaderTest,
    ::testing::Values(10, 50, 100, 1000)
);
