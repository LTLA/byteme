#include <gtest/gtest.h>

#include "read_lines.h"
#include "byteme/temp_file_path.hpp"
#include "byteme/RawFileReader.hpp"
#include "byteme/SomeFileReader.hpp"
#include <fstream>

class RawFileReaderTest : public ::testing::TestWithParam<int> {
protected:    
    auto dump_file(const std::vector<std::string>& contents) {
        auto path = byteme::temp_file_path("text");
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

    byteme::RawFileReader reader(path.c_str(), [&]{
        byteme::RawFileReaderOptions ropt;
        ropt.buffer_size = GetParam();
        return ropt;
    }());

    auto lines = read_lines(reader);
    EXPECT_EQ(lines, contents);
}

TEST_P(RawFileReaderTest, Empty) {
    std::vector<std::string> contents { "asdasdasd", "", "", "caysctgatctv", "", "", "((&9KKJNJSNAKASd", "" };
    auto path = dump_file(contents);

    byteme::RawFileReader reader(path.c_str(), [&]{
        byteme::RawFileReaderOptions ropt;
        ropt.buffer_size = GetParam();
        return ropt;
    }());

    auto lines = read_lines(reader);
    EXPECT_EQ(lines, contents);
}

TEST_P(RawFileReaderTest, TooLong) {
    std::vector<std::string> contents { "asdasdasd", "asdaisdaioufhiuvhdsiug sifyw983r7w9fsoiufhsiud nse98 98eye9s8fy siufhsu caysctgatctv", "((&9KKJNJSNAKASd" };
    auto path = dump_file(contents);

    byteme::RawFileReader reader(path.c_str(), [&]{
        byteme::RawFileReaderOptions ropt;
        ropt.buffer_size = GetParam();
        return ropt;
    }());

    auto lines = read_lines(reader);
    EXPECT_EQ(lines, contents);
}

TEST_P(RawFileReaderTest, SomeFileWorks) {
    std::vector<std::string> contents { "asdasdasd", "sd738", "93879sdjfsjdf", "caysctgatctv", "oirtueorpr2312", "09798&A*&^&c", "((&9KKJNJSNAKASd" };
    auto path = dump_file(contents);

    byteme::SomeFileReader reader(path.c_str(), [&]{
        byteme::SomeFileReaderOptions ropt;
        ropt.buffer_size = GetParam();
        return ropt;
    }());

    auto lines = read_lines(reader);
    EXPECT_EQ(lines, contents);
}

INSTANTIATE_TEST_SUITE_P(
    RawFileReader,
    RawFileReaderTest,
    ::testing::Values(10, 50, 100, 1000)
);

TEST_F(RawFileReaderTest, Moveable) {
    std::vector<std::string> contents { "asdasdasd", "sd738", "93879sdjfsjdf", "caysctgatctv", "oirtueorpr2312", "09798&A*&^&c", "((&9KKJNJSNAKASd" };
    auto path = dump_file(contents);

    // Move constructor.
    {
        byteme::RawFileReader reader(path.c_str(), {});
        byteme::RawFileReader other(std::move(reader));
        auto lines = read_lines(other);
        EXPECT_EQ(lines, contents);
    }

    // Move assignment.
    {
        byteme::RawFileReader reader(path.c_str(), {});
        byteme::RawFileReader other = std::move(reader);
        auto lines = read_lines(other);
        EXPECT_EQ(lines, contents);
    }
}
