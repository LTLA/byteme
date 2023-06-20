#include <gtest/gtest.h>

#include "read_lines.h"
#include "byteme/OstreamWriter.hpp"
#include "byteme/temp_file_path.hpp"

#include <fstream>

class OstreamWriterTest : public ::testing::Test {
protected:    
    auto dump_file(const std::vector<std::string>& contents) {
        auto path = buffin::temp_file_path("text");
        std::ofstream ostr(path);
        byteme::OstreamWriter writer(&ostr);

        const char newline = '\n';
        for (const auto& c : contents) {
            writer.write(reinterpret_cast<const unsigned char*>(c.c_str()), c.size());
            writer.write(reinterpret_cast<const unsigned char*>(&newline), 1);
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
