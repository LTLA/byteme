#include <gtest/gtest.h>

#include "read_lines.h"

#include "byteme/temp_file_path.hpp"
#include "byteme/SelfClosingGzFile.hpp"
#include "byteme/GzipFileWriter.hpp"

#include "zlib.h"
#include <fstream>

class GzipFileWriterTest : public ::testing::TestWithParam<int> {
protected:
    auto dump_file(const std::vector<std::string>& contents, size_t buffer_size) {
        auto path = byteme::temp_file_path("text");
        byteme::GzipFileWriter writer(path.c_str(), [&]{
            byteme::GzipFileWriterOptions opt;
            opt.buffer_size = buffer_size;
            return opt;
        }());

        for (const auto& c : contents) {
            writer.write(c);
            writer.write('\n');
        }
        writer.finish();
        return path;
    }

    static std::string zcat(const std::string& path) { 
        auto handle = gzopen(path.c_str(), "r");
        std::vector<unsigned char> everything(10000); // some large number.
        auto read = gzread(handle, everything.data(), everything.size());
        auto ptr = reinterpret_cast<const char*>(everything.data());
        return std::string(ptr, ptr + read);
    }

    static std::string combine(const std::vector<std::string>& contents) {
        std::string ex;
        for (const auto& x : contents) {
            ex += x;
            ex += '\n';
        }
        return ex;
    }
};

TEST_P(GzipFileWriterTest, Basic) {
    std::vector<std::string> contents { "asdasdasd", "sd738", "93879sdjfsjdf", "caysctgatctv", "oirtueorpr2312", "09798&A*&^&c", "((&9KKJNJSNAKASd" };
    auto path = dump_file(contents, GetParam());
    auto expected = combine(contents);
    auto roundtrip = zcat(path);
    EXPECT_EQ(roundtrip, expected);
}

TEST_P(GzipFileWriterTest, Empty) {
    std::vector<std::string> contents { "asdasdasd", "", "", "caysctgatctv", "", "", "((&9KKJNJSNAKASd", "" };
    auto path = dump_file(contents, GetParam());
    auto expected = combine(contents);
    auto roundtrip = zcat(path);
    EXPECT_EQ(roundtrip, expected);
}

TEST_P(GzipFileWriterTest, TooLong) {
    std::vector<std::string> contents { "asdasdasd", "asdaisdaioufhiuvhdsiug sifyw983r7w9fsoiufhsiud nse98 98eye9s8fy siufhsu caysctgatctv", "((&9KKJNJSNAKASd" };
    auto path = dump_file(contents, GetParam());
    auto expected = combine(contents);
    auto roundtrip = zcat(path);
    EXPECT_EQ(roundtrip, expected);
}

INSTANTIATE_TEST_SUITE_P(
    GzipFileWriter,
    GzipFileWriterTest,
    ::testing::Values(10, 50, 100, 1000)
);
