#include <gtest/gtest.h>

#include "LineReader.h"

#include "buffin/temp_file_path.hpp"
#include "buffin/parse_gzip_file.hpp"
#include "buffin/parse_some_file.hpp"

#include "zlib.h"
#include <fstream>

class ParseGzipFileTest : public ::testing::TestWithParam<int> {
protected:
    auto dump_file(const std::vector<std::string>& contents) {
        std::string stuff;
        for (auto c : contents) {
            stuff += c;
            stuff += '\n';
        }

        auto gzname = buffin::temp_file_path("gzip");
        gzFile ohandle = gzopen(gzname.c_str(), "w");
        gzwrite(ohandle, stuff.c_str(), stuff.size());
        gzclose(ohandle);
        return gzname;
    }
};

TEST_P(ParseGzipFileTest, Basic) {
    std::vector<std::string> contents { "asdasdasd", "sd738", "93879sdjfsjdf", "caysctgatctv", "oirtueorpr2312", "09798&A*&^&c", "((&9KKJNJSNAKASd" };
    auto path = dump_file(contents);
    LineReader reader;
    buffin::parse_gzip_file(path.c_str(), reader, GetParam());
    EXPECT_EQ(reader.lines, contents);
}

TEST_P(ParseGzipFileTest, Empty) {
    std::vector<std::string> contents { "asdasdasd", "", "", "caysctgatctv", "", "", "((&9KKJNJSNAKASd", "" };
    auto path = dump_file(contents);
    LineReader reader;
    buffin::parse_gzip_file(path.c_str(), reader, GetParam());
    EXPECT_EQ(reader.lines, contents);
}

TEST_P(ParseGzipFileTest, TooLong) {
    std::vector<std::string> contents { "asdasdasd", "asdaisdaioufhiuvhdsiug sifyw983r7w9fsoiufhsiud nse98 98eye9s8fy siufhsu caysctgatctv", "((&9KKJNJSNAKASd" };
    auto path = dump_file(contents);
    LineReader reader;
    buffin::parse_gzip_file(path.c_str(), reader, GetParam());
    EXPECT_EQ(reader.lines, contents);
}

TEST_P(ParseGzipFileTest, SomeFileWorks) {
    std::vector<std::string> contents { "asdasdasd", "sd738", "93879sdjfsjdf", "caysctgatctv", "oirtueorpr2312", "09798&A*&^&c", "((&9KKJNJSNAKASd" };
    auto path = dump_file(contents);
    LineReader reader;
    buffin::parse_some_file(path.c_str(), reader, GetParam());
    EXPECT_EQ(reader.lines, contents);
}

INSTANTIATE_TEST_SUITE_P(
    ParseGzipFile,
    ParseGzipFileTest,
    ::testing::Values(10, 50, 100, 1000)
);
