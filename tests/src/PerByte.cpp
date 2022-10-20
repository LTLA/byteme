#include <gtest/gtest.h>

#include "byteme/PerByte.hpp"
#include "byteme/temp_file_path.hpp"
#include "byteme/RawFileReader.hpp"
#include <fstream>

class PerByteTest : public ::testing::TestWithParam<int> {
protected:
    auto dump_file(const std::vector<std::string>& contents) {
        auto path = buffin::temp_file_path("text");
        std::ofstream output(path);
        for (auto c : contents) {
            output << c << "\n";
        }
        output.close();
        return path;
    }
};

TEST_P(PerByteTest, Basic) {
    std::vector<std::string> contents { "asdasdasd", "sd738", "93879sdjfsjdf", "caysctgatctv", "oirtueorpr2312", "09798&A*&^&c", "((&9KKJNJSNAKASd" };
    auto path = dump_file(contents);

    byteme::RawFileReader reader(path, GetParam());
    byteme::PerByte extractor(reader);

    std::string observed;
    size_t count = 0;
    std::vector<int> positions;
    while (extractor.valid()) {
        if (extractor.get() == '\n') {
            positions.push_back(extractor.position());
        } else {
            observed += extractor.get();
        }
        extractor.advance();
    }

    std::string expected;
    std::vector<int> expected_pos;
    int counter = 0;
    for (const auto& x : contents) {
        expected += x;
        counter += x.size(); // position is 0-based, so this implicitly includes the newline.
        expected_pos.push_back(counter);
        ++counter; // for the extra length due to the newline.
    }

    EXPECT_EQ(positions, expected_pos);
    EXPECT_EQ(observed, expected);
}

INSTANTIATE_TEST_SUITE_P(
    PerByte,
    PerByteTest,
    ::testing::Values(10, 50, 100, 1000)
);
