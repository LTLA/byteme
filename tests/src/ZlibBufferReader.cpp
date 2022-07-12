#include <gtest/gtest.h>

#include "read_lines.h"

#include "byteme/temp_file_path.hpp"
#include "byteme/ZlibBufferReader.hpp"
#include "byteme/SomeBufferReader.hpp"
#include "zlib.h"
#include <fstream>

class ZlibBufferReaderTest : public ::testing::TestWithParam<int> {
protected:
    std::vector<unsigned char> dump_file(const std::vector<std::string>& contents) {
        std::string stuff;
        for (auto c : contents) {
            stuff += c;
            stuff += '\n';
        }

        auto gzname = buffin::temp_file_path("zlib");
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

TEST_P(ZlibBufferReaderTest, Basic) {
    std::vector<std::string> contents { "asdasdasd", "sd738", "93879sdjfsjdf", "caysctgatctv", "oirtueorpr2312", "09798&A*&^&c", "((&9KKJNJSNAKASd" };
    auto gzcontents = dump_file(contents);

    byteme::ZlibBufferReader reader(gzcontents.data(), gzcontents.size(), 3, GetParam());
    auto lines = read_lines(reader);
    EXPECT_EQ(lines, contents);
}

TEST_P(ZlibBufferReaderTest, Empty) {
    std::vector<std::string> contents { "asdasdasd", "", "", "caysctgatctv", "", "", "((&9KKJNJSNAKASd", "" };
    auto gzcontents = dump_file(contents);

    byteme::ZlibBufferReader reader(gzcontents.data(), gzcontents.size(), 3, GetParam());
    auto lines = read_lines(reader);
    EXPECT_EQ(lines, contents);
}

TEST_P(ZlibBufferReaderTest, TooLong) {
    std::vector<std::string> contents { "asdasdasd", "asdaisdaioufhiuvhdsiug sifyw983r7w9fsoiufhsiud nse98 98eye9s8fy siufhsu caysctgatctv", "((&9KKJNJSNAKASd" };
    auto gzcontents = dump_file(contents);

    byteme::ZlibBufferReader reader(gzcontents.data(), gzcontents.size(), 3, GetParam());
    auto lines = read_lines(reader);
    EXPECT_EQ(lines, contents);
}

TEST_P(ZlibBufferReaderTest, SomeBufferWorks) {
    std::vector<std::string> contents { "asdasdasd", "asdaisdaioufhiuvhdsiug sifyw983r7w9fsoiufhsiud nse98 98eye9s8fy siufhsu caysctgatctv", "((&9KKJNJSNAKASd" };
    auto gzcontents = dump_file(contents);

    byteme::SomeBufferReader reader(gzcontents.data(), gzcontents.size(), GetParam());
    auto lines = read_lines(reader);
    EXPECT_EQ(lines, contents);
}

INSTANTIATE_TEST_SUITE_P(
    ZlibBufferReader,
    ZlibBufferReaderTest,
    ::testing::Values(10, 50, 100, 1000)
);
