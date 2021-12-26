#include <gtest/gtest.h>

#include "LineReader.h"
#include "temp_file_path.h"

#include "buffin/parse_zlib_buffer.hpp"
#include "zlib.h"
#include <fstream>

class ParseZlibBufferTest : public ::testing::TestWithParam<int> {
protected:
    std::vector<unsigned char> dump_file(const std::vector<std::string>& contents) {
        std::string stuff;
        for (auto c : contents) {
            stuff += c;
            stuff += '\n';
        }

        auto gzname = temp_file_path("zlib");
        gzFile ohandle = gzopen(gzname.c_str(), "w");
        gzwrite(ohandle, stuff.c_str(), stuff.size());
        gzclose(ohandle);

        std::ifstream in(gzname, std::ios::binary);
        if (!in) {
            throw std::runtime_error("failed to read '" + gzname + "'");
        }
        return std::vector<unsigned char>(std::istreambuf_iterator<char>(in), {});
    }
};

TEST_P(ParseZlibBufferTest, Basic) {
    std::vector<std::string> contents { "asdasdasd", "sd738", "93879sdjfsjdf", "caysctgatctv", "oirtueorpr2312", "09798&A*&^&c", "((&9KKJNJSNAKASd" };
    auto gzcontents = dump_file(contents);
    LineReader reader;
    buffin::parse_zlib_buffer(gzcontents.data(), gzcontents.size(), reader, 3, GetParam());
    EXPECT_EQ(reader.lines, contents);
}

TEST_P(ParseZlibBufferTest, Empty) {
    std::vector<std::string> contents { "asdasdasd", "", "", "caysctgatctv", "", "", "((&9KKJNJSNAKASd", "" };
    auto gzcontents = dump_file(contents);
    LineReader reader;
    buffin::parse_zlib_buffer(gzcontents.data(), gzcontents.size(), reader, 3, GetParam());
    EXPECT_EQ(reader.lines, contents);
}

TEST_P(ParseZlibBufferTest, TooLong) {
    std::vector<std::string> contents { "asdasdasd", "asdaisdaioufhiuvhdsiug sifyw983r7w9fsoiufhsiud nse98 98eye9s8fy siufhsu caysctgatctv", "((&9KKJNJSNAKASd" };
    auto gzcontents = dump_file(contents);
    LineReader reader;
    buffin::parse_zlib_buffer(gzcontents.data(), gzcontents.size(), reader, 3, GetParam());
    EXPECT_EQ(reader.lines, contents);
}

INSTANTIATE_TEST_SUITE_P(
    ParseZlibBuffer,
    ParseZlibBufferTest,
    ::testing::Values(10, 50, 100, 1000)
);
