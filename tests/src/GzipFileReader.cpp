#include <gtest/gtest.h>

#include "read_lines.h"
#include "temp_file_path.h"

#include "byteme/GzipFileReader.hpp"

#include "zlib.h"
#include <fstream>

class GzipFileReaderTest : public ::testing::TestWithParam<int> {
protected:
    auto dump_file(const std::vector<std::string>& contents) {
        std::string stuff;
        for (auto c : contents) {
            stuff += c;
            stuff += '\n';
        }

        auto gzname = temp_file_path("gzip");
        gzFile ohandle = gzopen(gzname.c_str(), "w");
        gzwrite(ohandle, stuff.c_str(), stuff.size());
        gzclose(ohandle);
        return gzname;
    }
};

TEST_P(GzipFileReaderTest, Basic) {
    std::vector<std::string> contents { "asdasdasd", "sd738", "93879sdjfsjdf", "caysctgatctv", "oirtueorpr2312", "09798&A*&^&c", "((&9KKJNJSNAKASd" };
    auto path = dump_file(contents);

    byteme::GzipFileReader reader(path.c_str(), {});
    auto lines = read_lines(reader, GetParam());
    EXPECT_EQ(lines, contents);
}

TEST_P(GzipFileReaderTest, Empty) {
    std::vector<std::string> contents;
    auto path = dump_file(contents);

    byteme::GzipFileReader reader(path.c_str(), {});
    auto lines = read_lines(reader, GetParam());
    EXPECT_EQ(lines, contents);
}

TEST_P(GzipFileReaderTest, TooLong) {
    std::vector<std::string> contents;
    for (int i = 0; i < 10; ++i) {
        contents.emplace_back(GetParam() - 1, char(i + 'a'));
    }
    auto path = dump_file(contents);

    byteme::GzipFileReader reader(path.c_str(), {});
    auto lines = read_lines(reader, GetParam());
    EXPECT_EQ(lines, contents);
}

INSTANTIATE_TEST_SUITE_P(
    GzipFileReader,
    GzipFileReaderTest,
    ::testing::Values(10, 20, 50, 100)
);

TEST_F(GzipFileReaderTest, Moveable) {
    std::vector<std::string> contents { "asdasdasd", "sd738", "93879sdjfsjdf", "caysctgatctv", "oirtueorpr2312", "09798&A*&^&c", "((&9KKJNJSNAKASd" };
    auto path = dump_file(contents);

    // Move constructor.
    {
        byteme::GzipFileReader reader(path.c_str(), {});
        byteme::GzipFileReader other(std::move(reader));
        auto lines = read_lines(other, 13);
        EXPECT_EQ(lines, contents);
    }

    // Move assignment.
    {
        byteme::GzipFileReader reader(path.c_str(), {});
        byteme::GzipFileReader other = std::move(reader);
        auto lines = read_lines(other, 22);
        EXPECT_EQ(lines, contents);
    }
}
