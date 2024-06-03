#include <gtest/gtest.h>

#include "byteme/RawBufferWriter.hpp"

class RawBufferWriterTest : public ::testing::Test {
protected:
    std::vector<unsigned char> roundtrip(const std::vector<std::string>& contents) {
        byteme::RawBufferWriter writer;
        for (const auto& x : contents) {
            writer.write(x);
            writer.write('\n');
        }
        writer.finish();
        return writer.get_output();
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

    auto output = roundtrip(contents);
    auto y = reinterpret_cast<const char*>(output.data());
    std::string combined(y, y + output.size());

    EXPECT_EQ(combined, expected);
}

TEST_F(RawBufferWriterTest, Empty) {
    std::vector<std::string> contents { "asdasdasd", "", "", "caysctgatctv", "", "", "((&9KKJNJSNAKASd", "" };
    auto expected = combine(contents);

    auto output = roundtrip(contents);
    auto y = reinterpret_cast<const char*>(output.data());
    std::string combined(y, y + output.size());

    EXPECT_EQ(combined, expected);
}

TEST_F(RawBufferWriterTest, ArrayCheck) {
    byteme::RawBufferWriter writer;
    std::string expected("Gloria in excelsis deo");
    writer.write(expected.c_str());
    writer.finish();

    const auto& stuff = writer.get_output();
    auto ptr = reinterpret_cast<const char*>(stuff.data());
    std::string extracted(ptr, ptr + stuff.size());

    EXPECT_EQ(extracted, expected);
}
