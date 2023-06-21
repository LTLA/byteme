#include <gtest/gtest.h>

#include "read_lines.h"

#include "byteme/temp_file_path.hpp"
#include "byteme/GzipFileReader.hpp"
#include "byteme/SomeFileReader.hpp"

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

        auto gzname = byteme::temp_file_path("gzip");
        gzFile ohandle = gzopen(gzname.c_str(), "w");
        gzwrite(ohandle, stuff.c_str(), stuff.size());
        gzclose(ohandle);
        return gzname;
    }
};

TEST_P(GzipFileReaderTest, Basic) {
    std::vector<std::string> contents { "asdasdasd", "sd738", "93879sdjfsjdf", "caysctgatctv", "oirtueorpr2312", "09798&A*&^&c", "((&9KKJNJSNAKASd" };
    auto path = dump_file(contents);

    byteme::GzipFileReader reader(path, GetParam());
    auto lines = read_lines(reader);
    EXPECT_EQ(lines, contents);
}

TEST_P(GzipFileReaderTest, Empty) {
    std::vector<std::string> contents { "asdasdasd", "", "", "caysctgatctv", "", "", "((&9KKJNJSNAKASd", "" };
    auto path = dump_file(contents);

    byteme::GzipFileReader reader(path, GetParam());
    auto lines = read_lines(reader);
    EXPECT_EQ(lines, contents);
}

TEST_P(GzipFileReaderTest, TooLong) {
    std::vector<std::string> contents { "asdasdasd", "asdaisdaioufhiuvhdsiug sifyw983r7w9fsoiufhsiud nse98 98eye9s8fy siufhsu caysctgatctv", "((&9KKJNJSNAKASd" };
    auto path = dump_file(contents);

    byteme::GzipFileReader reader(path, GetParam());
    auto lines = read_lines(reader);
    EXPECT_EQ(lines, contents);
}

TEST_P(GzipFileReaderTest, SomeFileWorks) {
    std::vector<std::string> contents { "asdasdasd", "sd738", "93879sdjfsjdf", "caysctgatctv", "oirtueorpr2312", "09798&A*&^&c", "((&9KKJNJSNAKASd" };
    auto path = dump_file(contents);

    byteme::SomeFileReader reader(path, GetParam());
    auto lines = read_lines(reader);
    EXPECT_EQ(lines, contents);
}

INSTANTIATE_TEST_SUITE_P(
    GzipFileReader,
    GzipFileReaderTest,
    ::testing::Values(10, 50, 100, 1000)
);

TEST_F(GzipFileReaderTest, Moveable) {
    std::vector<std::string> contents { "asdasdasd", "sd738", "93879sdjfsjdf", "caysctgatctv", "oirtueorpr2312", "09798&A*&^&c", "((&9KKJNJSNAKASd" };
    auto path = dump_file(contents);

    // Move constructor.
    {
        byteme::GzipFileReader reader(path, 100);
        byteme::GzipFileReader other(std::move(reader));
        auto lines = read_lines(other);
        EXPECT_EQ(lines, contents);
    }

    // Move assignment.
    {
        byteme::GzipFileReader reader(path, 100);
        byteme::GzipFileReader other = std::move(reader);
        auto lines = read_lines(other);
        EXPECT_EQ(lines, contents);
    }
}
