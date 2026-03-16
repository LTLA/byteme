#include <gtest/gtest.h>

#include "temp_file_path.h"
#include "utils.h"

#include "byteme/BufferedReader.hpp"
#include "byteme/RawBufferReader.hpp"

#include <fstream>
#include <memory>

class PerByteGetTest : public ::testing::TestWithParam<std::tuple<int, int, bool> > {};

TEST_P(PerByteGetTest, Basic) {
    auto param = GetParam();
    auto nbytes = std::get<0>(param);
    auto buffer_size = std::get<1>(param);
    auto parallel = std::get<2>(param);

    auto contents = simulate_bytes(nbytes, /* seed = */ 13 * nbytes + buffer_size + parallel);
    byteme::RawBufferReader reader(contents.data(), contents.size());

    std::unique_ptr<byteme::BufferedReader<unsigned char> > ptr;
    if (parallel) {
        ptr.reset(new byteme::ParallelBufferedReader<unsigned char, byteme::Reader*>(&reader, buffer_size));
    } else {
        ptr.reset(new byteme::SerialBufferedReader<unsigned char, byteme::Reader*>(&reader, buffer_size));
    }

    std::vector<unsigned char> output;
    while (ptr->valid()) {
        EXPECT_EQ(ptr->position(), output.size());
        output.push_back(ptr->get());
        bool still_valid = ptr->advance();
        EXPECT_EQ(still_valid, ptr->valid());
    }

    EXPECT_EQ(output, contents);
}

// We want to make sure we test cases where the buffer size is a factor of, greater than or smaller than the total number of bytes.
// This gets some test coverage on the various scenarios in BufferedReader::advance().
INSTANTIATE_TEST_SUITE_P(
    PerByte,
    PerByteGetTest,
    ::testing::Combine(
        ::testing::Values(100, 256, 500, 1024), // number of bytes
        ::testing::Values(50, 128, 200, 512), // buffer size
        ::testing::Values(false, true) // parallel or not
    )
);

class PerByteExtractTest : public ::testing::TestWithParam<std::tuple<int, int, int, bool> > {};

TEST_P(PerByteExtractTest, Basic) {
    auto param = GetParam();
    auto nbytes = std::get<0>(param);
    auto buffer_size = std::get<1>(param);
    auto extract_len = std::get<2>(param);
    auto parallel = std::get<3>(param);

    auto contents = simulate_bytes(nbytes, /* seed = */ 42 * nbytes + buffer_size + extract_len + parallel);
    auto xptr = std::make_unique<byteme::RawBufferReader>(contents.data(), contents.size()); // Using smart pointers for some variety.

    std::unique_ptr<byteme::BufferedReader<unsigned char> > ptr;
    if (parallel) {
        ptr.reset(new byteme::ParallelBufferedReader<unsigned char, std::unique_ptr<byteme::Reader> >(std::move(xptr), buffer_size));
    } else {
        ptr.reset(new byteme::SerialBufferedReader<unsigned char, std::unique_ptr<byteme::Reader> >(std::move(xptr), buffer_size));
    }

    std::vector<unsigned char> observed;
    std::vector<unsigned char> buffer(extract_len);
    while (1) {
        auto out = ptr->extract(buffer.size(), buffer.data());
        observed.insert(observed.end(), buffer.data(), buffer.data() + out.first);
        EXPECT_EQ(ptr->position(), observed.size());
        EXPECT_EQ(out.second, ptr->valid());
        if (!out.second) {
            break;
        }
    }

    EXPECT_EQ(observed, contents);
}

TEST_P(PerByteExtractTest, Mixed) {
    auto param = GetParam();
    auto nbytes = std::get<0>(param);
    auto buffer_size = std::get<1>(param);
    auto extract_len = std::get<2>(param);
    auto parallel = std::get<3>(param);

    auto contents = simulate_bytes(nbytes, /* seed = */ 42 * nbytes + buffer_size + extract_len + parallel);
    auto xptr = std::make_unique<byteme::RawBufferReader>(contents.data(), contents.size()); // Using smart pointers for some variety.

    std::unique_ptr<byteme::BufferedReader<unsigned char> > ptr;
    if (parallel) {
        ptr.reset(new byteme::ParallelBufferedReader<unsigned char, std::unique_ptr<byteme::Reader> >(std::move(xptr), buffer_size));
    } else {
        ptr.reset(new byteme::SerialBufferedReader<unsigned char, std::unique_ptr<byteme::Reader> >(std::move(xptr), buffer_size));
    }

    // Checking that extract() and get/advance() play nice together.
    std::vector<unsigned char> observed;
    std::vector<unsigned char> buffer(extract_len);
    while (1) {
        auto out = ptr->extract(buffer.size(), buffer.data());
        observed.insert(observed.end(), buffer.data(), buffer.data() + out.first);
        if (!out.second) {
            break;
        }

        for (int i = 0; i < extract_len; ++i) {
            observed.push_back(ptr->get());
            if (!ptr->advance()) {
                break;
            }
        }
        if (!ptr->valid()) {
            break;
        }
    }

    EXPECT_EQ(observed, contents);
}


// We want to make sure we test cases where the extraction length is a factor of, greater than or smaller than the Reader buffer size;
// and similarly, the buffer size is a factor of, greater than or smaller than the total number of bytes.
// This gets some test coverage on the various scenarios in BufferedReader::extract().
INSTANTIATE_TEST_SUITE_P(
    PerByte,
    PerByteExtractTest,
    ::testing::Combine(
        ::testing::Values(100, 256, 500, 1024), // number of bytes
        ::testing::Values(50, 128, 200, 512), // buffer size 
        ::testing::Values(25, 64, 128, 200), // extract len
        ::testing::Values(false, true) // parallel or not
    )
);

class PerByteCharTest : public ::testing::TestWithParam<bool> {};

TEST_P(PerByteCharTest, Basic) {
    auto parallel = GetParam();

    std::string contents(
        "asdasdasd"
        "sd738"
        "93879sdjfsjdf"
        "caysctgatctv"
        "oirtueorpr2312"
        "09798&A*&^&c"
        "((&9KKJNJSNAKASd"
    );

    for (int mode = 0; mode < 2; ++mode) {
        byteme::RawBufferReader reader(reinterpret_cast<const unsigned char*>(contents.c_str()), contents.size());

        // Checking it works with char types.
        std::unique_ptr<byteme::BufferedReader<char> > ptr;
        if (parallel) {
            ptr.reset(new byteme::ParallelBufferedReader<char, byteme::Reader*>(&reader, 10));
        } else {
            ptr.reset(new byteme::SerialBufferedReader<char, byteme::Reader*>(&reader, 10));
        }

        EXPECT_TRUE(ptr->valid());

        std::string output;
        if (mode == 0) {
            while (1) {
                output += ptr->get();
                if (!(ptr->advance())) {
                    break;
                }
            }
        } else {
            std::vector<char> buffer(7);
            while (1) {
                auto extracted = ptr->extract(buffer.size(), buffer.data());
                output.insert(output.end(), buffer.data(), buffer.data() + extracted.first);
                if (!extracted.second) {
                    break;
                }
            }
        }

        EXPECT_EQ(output, contents);
    }
}

INSTANTIATE_TEST_SUITE_P(
    PerByte,
    PerByteCharTest,
    ::testing::Values(false, true)
);
