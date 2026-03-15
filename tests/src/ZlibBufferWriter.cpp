#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "read_lines.h"
#include "temp_file_path.h"

#include "byteme/ZlibBufferWriter.hpp"
#include "byteme/ZlibBufferReader.hpp"
#include "zlib.h"

#include <fstream>

class ZlibBufferWriterTest : public ::testing::TestWithParam<std::tuple<byteme::ZlibCompressionMode, int, size_t> > {
protected:
    std::string roundtrip(const std::vector<std::string>& contents, byteme::ZlibCompressionMode mode, int level, size_t chunk) {
        byteme::ZlibBufferWriter writer([&]{
            byteme::ZlibBufferWriterOptions zopt;
            zopt.mode = mode;
            zopt.compression_level = level;
            zopt.buffer_size = chunk;
            return zopt;
        }());

        for (const auto& c : contents) {
            writer.write(c);
            writer.write('\n');
        }
        writer.finish();

        if (mode == byteme::ZlibCompressionMode::GZIP) {
            auto path = temp_file_path("temp_");
            {
                std::ofstream dump(path, std::ios::binary);
                const auto& output = writer.get_output();
                dump.write(reinterpret_cast<const char*>(output.data()), output.size());
            }

            gzFile ohandle = gzopen(path.c_str(), "r");
            std::vector<char> holding(10000);
            size_t avail = gzread(ohandle, reinterpret_cast<unsigned char*>(holding.data()), holding.size());
            gzclose(ohandle);

            return std::string(holding.data(), holding.data() + avail);

        } else {
            const auto& contents = writer.get_output();
            byteme::ZlibBufferReader reader(contents.data(), contents.size(), [&]{
                byteme::ZlibBufferReaderOptions zopt;
                zopt.mode = mode;
                return zopt;
            }());

            std::vector<unsigned char> buffer(100);
            std::string output;
            while (1) {
                auto nread = reader.read(buffer.data(), buffer.size());
                auto ptr = reinterpret_cast<const char*>(buffer.data());
                output.insert(output.end(), ptr, ptr + nread);
                if (nread < buffer.size()) {
                    break;
                }
            }

            return output;
        }
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
    auto mode = std::get<0>(param);
    auto level = std::get<1>(param);
    auto chunk = std::get<2>(param);

    auto observed = roundtrip(contents, mode, level, chunk);
    auto expected = combine(contents);
    EXPECT_EQ(observed, expected);
}

TEST_P(ZlibBufferWriterTest, Empty) {
    std::vector<std::string> contents { "asdasdasd", "", "", "caysctgatctv", "", "", "((&9KKJNJSNAKASd", "" };

    auto param = GetParam();
    auto mode = std::get<0>(param);
    auto level = std::get<1>(param);
    auto chunk = std::get<2>(param);

    auto observed = roundtrip(contents, mode, level, chunk);
    auto expected = combine(contents);
    EXPECT_EQ(observed, expected);
}

TEST_P(ZlibBufferWriterTest, TooLong) {
    std::vector<std::string> contents { "asdasdasd", "asdaisdaioufhiuvhdsiug sifyw983r7w9fsoiufhsiud nse98 98eye9s8fy siufhsu caysctgatctv", "((&9KKJNJSNAKASd" };

    auto param = GetParam();
    auto mode = std::get<0>(param);
    auto level = std::get<1>(param);
    auto chunk = std::get<2>(param);

    auto observed = roundtrip(contents, mode, level, chunk);
    auto expected = combine(contents);
    EXPECT_EQ(observed, expected);
}

INSTANTIATE_TEST_SUITE_P(
    ZlibBufferWriter,
    ZlibBufferWriterTest,
    ::testing::Combine(
        ::testing::Values(byteme::ZlibCompressionMode::DEFLATE, byteme::ZlibCompressionMode::ZLIB, byteme::ZlibCompressionMode::GZIP), // compression mode 
        ::testing::Values(1, 5, 9), // compression level
        ::testing::Values(10, 50, 100, 1000) // chunk size
    )
);
