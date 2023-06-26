#include <gtest/gtest.h>

#include "byteme/temp_file_path.hpp"
#include "byteme/RawFileWriter.hpp"
#include <fstream>

class RawFileWriterTest : public ::testing::TestWithParam<int> {
protected:
    auto dump_file(const std::vector<std::string>& contents, size_t chunk) {
        auto path = byteme::temp_file_path("text");
        byteme::RawFileWriter writer(path, chunk);
        for (const auto& c : contents) {
            writer.write(c);
            writer.write('\n');
        }
        writer.finish();
        return path;
    }

    std::string cat(const std::string& path) { 
        std::ifstream t(path);
        std::stringstream buffer;
        buffer << t.rdbuf();
        return buffer.str();
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

TEST_P(RawFileWriterTest, Basic) {
    std::vector<std::string> contents { "asdasdasd", "sd738", "93879sdjfsjdf", "caysctgatctv", "oirtueorpr2312", "09798&A*&^&c", "((&9KKJNJSNAKASd" };
    auto path = dump_file(contents, GetParam());
    auto roundtrip = cat(path);
    auto expected = combine(contents);
    EXPECT_EQ(expected, roundtrip);
}

TEST_P(RawFileWriterTest, Empty) {
    std::vector<std::string> contents { "asdasdasd", "", "", "caysctgatctv", "", "", "((&9KKJNJSNAKASd", "" };
    auto path = dump_file(contents, GetParam());
    auto roundtrip = cat(path);
    auto expected = combine(contents);
    EXPECT_EQ(expected, roundtrip);
}

TEST_P(RawFileWriterTest, TooLong) {
    std::vector<std::string> contents { "asdasdasd", "asdaisdaioufhiuvhdsiug sifyw983r7w9fsoiufhsiud nse98 98eye9s8fy siufhsu caysctgatctv", "((&9KKJNJSNAKASd" };
    auto path = dump_file(contents, GetParam());
    auto roundtrip = cat(path);
    auto expected = combine(contents);
}

INSTANTIATE_TEST_SUITE_P(
    RawFileWriter,
    RawFileWriterTest,
    ::testing::Values(10, 50, 100, 1000)
);
