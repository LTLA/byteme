#include <gtest/gtest.h>

#include "read_lines.h"
#include "temp_file_path.h"

#include "byteme/OstreamWriter.hpp"

#include <fstream>

class OstreamWriterTest : public ::testing::Test {
protected:    
    auto dump_file(const std::vector<std::string>& contents) {
        auto path = temp_file_path("text");
        byteme::OstreamWriter writer(std::make_unique<std::ofstream>(path));

        for (const auto& c : contents) {
            writer.write(c);
            writer.write('\n');
        }
        writer.finish();

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

TEST_F(OstreamWriterTest, Basic) {
    std::vector<std::string> contents { "asdasdasd", "sd738", "93879sdjfsjdf", "caysctgatctv", "oirtueorpr2312", "09798&A*&^&c", "((&9KKJNJSNAKASd" };
    auto roundtrip = dump_file(contents);
    auto expected = combine(contents);
    EXPECT_EQ(roundtrip, expected);
}

TEST_F(OstreamWriterTest, Empty) {
    std::vector<std::string> contents { "asdasdasd", "", "", "caysctgatctv", "", "", "((&9KKJNJSNAKASd", "" };
    auto roundtrip = dump_file(contents);
    auto expected = combine(contents);
    EXPECT_EQ(roundtrip, expected);
}

TEST_F(OstreamWriterTest, TooLong) {
    std::vector<std::string> contents { "asdasdasd", "asdaisdaioufhiuvhdsiug sifyw983r7w9fsoiufhsiud nse98 98eye9s8fy siufhsu caysctgatctv", "((&9KKJNJSNAKASd" };
    auto roundtrip = dump_file(contents);
    auto expected = combine(contents);
    EXPECT_EQ(roundtrip, expected);
}
