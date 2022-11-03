#include <gtest/gtest.h>

#include "byteme/RawBufferWriter.hpp"

class RawBufferWriterTest : public ::testing::Test {
protected:
    std::vector<unsigned char> roundtrip(const std::vector<std::string>& contents) {
        byteme::RawBufferWriter writer;
        const char newline = '\n';
        for (const auto& x : contents) {
            writer.write(reinterpret_cast<const unsigned char*>(x.c_str()), x.size());
            writer.write(reinterpret_cast<const unsigned char*>(&newline), 1);
        }
        writer.finish();
        return writer.contents;
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

TEST_F(RawBufferWriterTest, Basic) {
    std::vector<std::string> contents { "asdasdasd", "sd738", "93879sdjfsjdf", "caysctgatctv", "oirtueorpr2312", "09798&A*&^&c", "((&9KKJNJSNAKASd" };
    auto expected = combine(contents);

    const auto& output = roundtrip(contents);
    auto y = reinterpret_cast<const char*>(output.data());
    std::string combined(y, y + output.size());

    EXPECT_EQ(combined, expected);
}

TEST_F(RawBufferWriterTest, Empty) {
    std::vector<std::string> contents { "asdasdasd", "", "", "caysctgatctv", "", "", "((&9KKJNJSNAKASd", "" };
    auto expected = combine(contents);

    const auto& output = roundtrip(contents);
    auto y = reinterpret_cast<const char*>(output.data());
    std::string combined(y, y + output.size());

    EXPECT_EQ(combined, expected);
}
