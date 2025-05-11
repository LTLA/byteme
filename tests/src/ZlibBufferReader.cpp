#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "read_lines.h"
#include "temp_file_path.h"

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

TEST_P(ZlibBufferReaderTest, Basic) {
    std::vector<std::string> contents { "asdasdasd", "sd738", "93879sdjfsjdf", "caysctgatctv", "oirtueorpr2312", "09798&A*&^&c", "((&9KKJNJSNAKASd" };
    auto gzcontents = dump_file(contents);

    byteme::ZlibBufferReader reader(gzcontents.data(), gzcontents.size(), [&]{
        byteme::ZlibBufferReaderOptions zopt;
        zopt.buffer_size = GetParam();
        return zopt;
    }());

    auto lines = read_lines(reader);
    EXPECT_EQ(lines, contents);

    // Trying in dedicated Gzip mode.
    {
        byteme::ZlibBufferReader reader(gzcontents.data(), gzcontents.size(), [&]{
            byteme::ZlibBufferReaderOptions zopt;
            zopt.mode = 2;
            zopt.buffer_size = GetParam();
            return zopt;
        }());

        auto lines = read_lines(reader);
        EXPECT_EQ(lines, contents);
    }
}

TEST_P(ZlibBufferReaderTest, Empty) {
    std::vector<std::string> contents { "asdasdasd", "", "", "caysctgatctv", "", "", "((&9KKJNJSNAKASd", "" };
    auto gzcontents = dump_file(contents);

    byteme::ZlibBufferReader reader(gzcontents.data(), gzcontents.size(), [&]{
        byteme::ZlibBufferReaderOptions zopt;
        zopt.buffer_size = GetParam();
        return zopt;
    }());

    auto lines = read_lines(reader);
    EXPECT_EQ(lines, contents);
}

TEST_P(ZlibBufferReaderTest, TooLong) {
    std::vector<std::string> contents { "asdasdasd", "asdaisdaioufhiuvhdsiug sifyw983r7w9fsoiufhsiud nse98 98eye9s8fy siufhsu caysctgatctv", "((&9KKJNJSNAKASd" };
    auto gzcontents = dump_file(contents);

    byteme::ZlibBufferReader reader(gzcontents.data(), gzcontents.size(), [&]{
        byteme::ZlibBufferReaderOptions zopt;
        zopt.buffer_size = GetParam();
        return zopt;
    }());

    auto lines = read_lines(reader);
    EXPECT_EQ(lines, contents);
}

TEST_P(ZlibBufferReaderTest, SomeBufferWorks) {
    std::vector<std::string> contents { "asdasdasd", "asdaisdaioufhiuvhdsiug sifyw983r7w9fsoiufhsiud nse98 98eye9s8fy siufhsu caysctgatctv", "((&9KKJNJSNAKASd" };
    auto gzcontents = dump_file(contents);

    byteme::SomeBufferReader reader(gzcontents.data(), gzcontents.size(), [&]{
        byteme::SomeBufferReaderOptions sopt;
        sopt.buffer_size = GetParam();
        return sopt;
    }());

    auto lines = read_lines(reader);
    EXPECT_EQ(lines, contents);
}

INSTANTIATE_TEST_SUITE_P(
    ZlibBufferReader,
    ZlibBufferReaderTest,
    ::testing::Values(10, 50, 100, 1000)
);

TEST(ZlibBufferReaderExtraTests, OtherModes) {
    byteme::ZlibBufferReader reader1(NULL, 0, [&]{
        byteme::ZlibBufferReaderOptions zopt;
        zopt.mode = 0; // deflate
        return zopt;
    }());

    byteme::ZlibBufferReader reader2(NULL, 0, [&]{
        byteme::ZlibBufferReaderOptions zopt;
        zopt.mode = 1; // zlib
        return zopt;
    }());

    EXPECT_ANY_THROW(
        try {
            byteme::ZlibBufferReader reader(NULL, 0, [&]{
                byteme::ZlibBufferReaderOptions zopt;
                zopt.mode = 5; 
                return zopt;
            }());
        } catch (std::exception& e) {
            EXPECT_THAT(e.what(), ::testing::HasSubstr("mode must be"));
            throw e;
        }
    );
}
