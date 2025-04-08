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
        byteme::ZlibBufferWriter writer([&]{
            byteme::ZlibBufferWriterOptions zopt;
            zopt.mode = 2;
            zopt.compression_level = level;
            zopt.buffer_size = chunk;
            return zopt;
        }());

        for (const auto& c : contents) {
            writer.write(c);
            writer.write('\n');
        }
        writer.finish();

        auto path = byteme::temp_file_path("temp_");
        {
            std::ofstream dump(path, std::ios::binary);
            auto& output = writer.get_output();
            dump.write(reinterpret_cast<const char*>(output.data()), output.size());
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
    byteme::ZlibBufferWriter writer1([&]{
        byteme::ZlibBufferWriterOptions opt;
        opt.mode = 0; // deflate
        return opt;
    }());

    byteme::ZlibBufferWriter writer2([&]{
        byteme::ZlibBufferWriterOptions opt;
        opt.mode = 1; // zlib
        return opt;
    }());

    EXPECT_ANY_THROW(
        try {
            byteme::ZlibBufferWriter writer([&]{
                byteme::ZlibBufferWriterOptions opt;
                opt.mode = 5;
                return opt;
            }());
        } catch (std::exception& e) {
            EXPECT_THAT(e.what(), ::testing::HasSubstr("unknown Zlib compression"));
            throw e;
        }
    );
}

