#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "read_lines.h"

#include "byteme/temp_file_path.hpp"
#include "byteme/ZlibBufferWriter.hpp"
#include "zlib.h"
#include <fstream>

class ZlibBufferWriterTest : public ::testing::TestWithParam<std::tuple<int, size_t> > {
protected:
    std::string roundtrip(const std::vector<std::string>& contents, int level, size_t chunk) {
        byteme::ZlibBufferWriter writer(2, level, chunk);

        constexpr char newline = '\n';
        for (const auto& c : contents) {
            writer.write(reinterpret_cast<const unsigned char*>(c.c_str()), c.size());
            writer.write(reinterpret_cast<const unsigned char*>(&newline), 1);
        }
        writer.finish();

        auto path = byteme::temp_file_path("temp_");
        {
            std::ofstream dump(path, std::ios::binary);
            dump.write(reinterpret_cast<const char*>(writer.output.data()), writer.output.size());
        }

        gzFile ohandle = gzopen(path.c_str(), "r");
        std::vector<char> holding(10000);
        size_t avail = gzread(ohandle, reinterpret_cast<unsigned char*>(holding.data()), holding.size());
        gzclose(ohandle);

        return std::string(holding.data(), holding.data() + avail);
    }

    std::string combine(const std::vector<std::string>& contents) {
        std::string ex;
        for (const auto& x : contents) {
            ex += x;
            ex += '\n';
        }
        return ex;
    }
};

TEST_P(ZlibBufferWriterTest, Basic) {
    std::vector<std::string> contents { "asdasdasd", "sd738", "93879sdjfsjdf", "caysctgatctv", "oirtueorpr2312", "09798&A*&^&c", "((&9KKJNJSNAKASd" };

    auto param = GetParam();
    auto level = std::get<0>(param);
    auto chunk = std::get<1>(param);

    auto observed = roundtrip(contents, level, chunk);
    auto expected = combine(contents);
    EXPECT_EQ(observed, expected);
}

TEST_P(ZlibBufferWriterTest, Empty) {
    std::vector<std::string> contents { "asdasdasd", "", "", "caysctgatctv", "", "", "((&9KKJNJSNAKASd", "" };

    auto param = GetParam();
    auto level = std::get<0>(param);
    auto chunk = std::get<1>(param);

    auto observed = roundtrip(contents, level, chunk);
    auto expected = combine(contents);
    EXPECT_EQ(observed, expected);
}

TEST_P(ZlibBufferWriterTest, TooLong) {
    std::vector<std::string> contents { "asdasdasd", "asdaisdaioufhiuvhdsiug sifyw983r7w9fsoiufhsiud nse98 98eye9s8fy siufhsu caysctgatctv", "((&9KKJNJSNAKASd" };

    auto param = GetParam();
    auto level = std::get<0>(param);
    auto chunk = std::get<1>(param);

    auto observed = roundtrip(contents, level, chunk);
    auto expected = combine(contents);
    EXPECT_EQ(observed, expected);
}

INSTANTIATE_TEST_SUITE_P(
    ZlibBufferWriter,
    ZlibBufferWriterTest,
    ::testing::Combine(
        ::testing::Values(1, 5, 9), // compression level
        ::testing::Values(10, 50, 100, 1000) // chunk size
    )
);

TEST(ZlibBufferWriterExtraTests, OtherModes) {
    byteme::ZlibBufferWriter writer1(0); // deflate
    byteme::ZlibBufferWriter writer2(1); // zlib

    EXPECT_ANY_THROW(
        try {
            byteme::ZlibBufferWriter writer(5);
        } catch (std::exception& e) {
            EXPECT_THAT(e.what(), ::testing::HasSubstr("unknown Zlib compression"));
            throw e;
        }
    );
}

