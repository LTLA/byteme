#include <gtest/gtest.h>

#include "read_lines.h"
#include "temp_file_path.h"

#include "byteme/RawFileReader.hpp"

#include <fstream>

class RawFileReaderTest : public ::testing::TestWithParam<int> {
protected:    
    auto dump_file(const std::vector<std::string>& contents) {
        auto path = temp_file_path("text");
        std::ofstream output(path);
        for (auto c : contents) {
            output << c << "\n";
        }
        output.close();
        return path;
    }
};

TEST_P(RawFileReaderTest, Basic) {
    std::vector<std::string> contents { "asdasdasd", "sd738", "93879sdjfsjdf", "caysctgatctv", "oirtueorpr2312", "09798&A*&^&c", "((&9KKJNJSNAKASd" };
    auto path = dump_file(contents);

    byteme::RawFileReader reader(path.c_str(), {});

    auto lines = read_lines(reader, GetParam());
    EXPECT_EQ(lines, contents);
}

TEST_P(RawFileReaderTest, Empty) {
    std::vector<std::string> contents;
    auto path = dump_file(contents);

    byteme::RawFileReader reader(path.c_str(), {});
    auto lines = read_lines(reader, GetParam());
    EXPECT_EQ(lines, contents);
}

TEST_P(RawFileReaderTest, Exact) {
    std::vector<std::string> contents;
    for (int i = 0; i < 10; ++i) {
        contents.emplace_back(GetParam() - 1, char(i + 'a'));
    }
    auto path = dump_file(contents);

    byteme::RawFileReader reader(path.c_str(), {});
    auto lines = read_lines(reader, GetParam());
    EXPECT_EQ(lines, contents);
}

INSTANTIATE_TEST_SUITE_P(
    RawFileReader,
    RawFileReaderTest,
    ::testing::Values(10, 20, 50, 100)
);

TEST_F(RawFileReaderTest, Moveable) {
    std::vector<std::string> contents { "asdasdasd", "sd738", "93879sdjfsjdf", "caysctgatctv", "oirtueorpr2312", "09798&A*&^&c", "((&9KKJNJSNAKASd" };
    auto path = dump_file(contents);

    // Move constructor.
    {
        byteme::RawFileReader reader(path.c_str(), {});
        byteme::RawFileReader other(std::move(reader));
        auto lines = read_lines(other, 15);
        EXPECT_EQ(lines, contents);
    }

    // Move assignment.
    {
        byteme::RawFileReader reader(path.c_str(), {});
        byteme::RawFileReader other = std::move(reader);
        auto lines = read_lines(other, 20);
        EXPECT_EQ(lines, contents);
    }
}
