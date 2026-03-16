#include <gtest/gtest.h>

#include "temp_file_path.h"
#include "utils.h"

#include "byteme/BufferedWriter.hpp"
#include "byteme/RawBufferWriter.hpp"

#include <fstream>
#include <memory>

class BufferedWriterSingleTest : public ::testing::TestWithParam<std::tuple<int, int, bool> > {};

TEST_P(BufferedWriterSingleTest, Basic) {
    auto param = GetParam();
    auto nbytes = std::get<0>(param);
    auto buffer_size = std::get<1>(param);
    auto parallel = std::get<2>(param);

    auto contents = simulate_bytes(nbytes, /* seed = */ 13 * nbytes + buffer_size + parallel);
    byteme::RawBufferWriter writer({});

    std::unique_ptr<byteme::BufferedWriter<unsigned char> > ptr;
    if (parallel) {
        ptr.reset(new byteme::ParallelBufferedWriter<unsigned char, byteme::Writer*>(&writer, buffer_size));
    } else {
        ptr.reset(new byteme::SerialBufferedWriter<unsigned char, byteme::Writer*>(&writer, buffer_size));
    }

    for (int b = 0; b < nbytes; ++b) {
        EXPECT_EQ(ptr->number(), b);
        ptr->write(contents[b]);
    }

    EXPECT_EQ(ptr->number(), nbytes);
    ptr->finish();
    EXPECT_EQ(writer.get_output(), contents);
}

TEST_P(BufferedWriterSingleTest, Flushing) {
    auto param = GetParam();
    auto nbytes = std::get<0>(param);
    auto buffer_size = std::get<1>(param);
    auto parallel = std::get<2>(param);

    auto contents = simulate_bytes(nbytes, /* seed = */ 13 * nbytes + buffer_size + parallel);
    byteme::RawBufferWriter writer({});

    std::unique_ptr<byteme::BufferedWriter<unsigned char> > ptr;
    if (parallel) {
        ptr.reset(new byteme::ParallelBufferedWriter<unsigned char, byteme::Writer*>(&writer, buffer_size));
    } else {
        ptr.reset(new byteme::SerialBufferedWriter<unsigned char, byteme::Writer*>(&writer, buffer_size));
    }

    // Flushing frequently and checking that it works.
    for (int b = 0; b < nbytes; ++b) {
        EXPECT_EQ(ptr->number(), b);
        ptr->write(contents[b]);
        ptr->flush();
    }

    ptr->flush(); // flushing again, for old-time's sake - this should be a no-op.
    EXPECT_EQ(ptr->number(), nbytes);
    ptr->finish();
    EXPECT_EQ(writer.get_output(), contents);
}

TEST_P(BufferedWriterSingleTest, AutoFlush) {
    auto param = GetParam();
    auto nbytes = std::get<0>(param);
    auto buffer_size = std::get<1>(param);
    auto parallel = std::get<2>(param);

    auto contents = simulate_bytes(nbytes, /* seed = */ 13 * nbytes + buffer_size + parallel);
    byteme::RawBufferWriter writer({});

    // Scoping so that the destructor is called before the test ends.
    // Don't bother calling BufferedWriter::flush() explicitly.
    // We want to check that the BufferedWriter automatically flushes upon its destruction.
    {
        std::unique_ptr<byteme::BufferedWriter<unsigned char> > ptr;
        if (parallel) {
            ptr.reset(new byteme::ParallelBufferedWriter<unsigned char, byteme::Writer*>(&writer, buffer_size));
        } else {
            ptr.reset(new byteme::SerialBufferedWriter<unsigned char, byteme::Writer*>(&writer, buffer_size));
        }

        // Flushing frequently and checking that it works.
        for (int b = 0; b < nbytes; ++b) {
            ptr->write(contents[b]);
        }
    }

    writer.finish();
    EXPECT_EQ(writer.get_output(), contents);
}

// We want to make sure we test cases where the buffer size is a factor of, greater than or smaller than the total number of bytes.
// This gets some test coverage on the various scenarios in BufferedWriter::advance().
INSTANTIATE_TEST_SUITE_P(
    BufferedWriter,
    BufferedWriterSingleTest,
    ::testing::Combine(
        ::testing::Values(100, 256, 500, 1024), // number of bytes
        ::testing::Values(50, 128, 200, 512), // buffer size
        ::testing::Values(false, true) // parallel or not
    )
);

class BufferedWriterArrayTest : public ::testing::TestWithParam<std::tuple<int, int, int, bool> > {};

TEST_P(BufferedWriterArrayTest, Basic) {
    auto param = GetParam();
    auto nbytes = std::get<0>(param);
    auto buffer_size = std::get<1>(param);
    auto extract_len = std::get<2>(param);
    auto parallel = std::get<3>(param);

    auto contents = simulate_bytes(nbytes, /* seed = */ 42 * nbytes + buffer_size + extract_len + parallel);
    auto xptr = std::make_shared<byteme::RawBufferWriter>(byteme::RawBufferWriterOptions()); // Using smart pointers for some variety.

    std::unique_ptr<byteme::BufferedWriter<unsigned char> > ptr;
    if (parallel) {
        ptr.reset(new byteme::ParallelBufferedWriter<unsigned char, std::shared_ptr<byteme::Writer> >(xptr, buffer_size));
    } else {
        ptr.reset(new byteme::SerialBufferedWriter<unsigned char, std::shared_ptr<byteme::Writer> >(xptr, buffer_size));
    }

    for (int b = 0; b < nbytes; b += extract_len) {
        EXPECT_EQ(ptr->number(), b);
        ptr->write(contents.data() + b, std::min(extract_len, nbytes - b));
    }

    ptr->finish();
    EXPECT_EQ(xptr->get_output(), contents);
}

TEST_P(BufferedWriterArrayTest, Mixed) {
    auto param = GetParam();
    auto nbytes = std::get<0>(param);
    auto buffer_size = std::get<1>(param);
    auto extract_len = std::get<2>(param);
    auto parallel = std::get<3>(param);

    auto contents = simulate_bytes(nbytes, /* seed = */ 42 * nbytes + buffer_size + extract_len + parallel);
    auto xptr = std::make_shared<byteme::RawBufferWriter>(byteme::RawBufferWriterOptions()); // Using smart pointers for some variety.

    std::unique_ptr<byteme::BufferedWriter<unsigned char> > ptr;
    if (parallel) {
        ptr.reset(new byteme::ParallelBufferedWriter<unsigned char, std::shared_ptr<byteme::Writer> >(xptr, buffer_size));
    } else {
        ptr.reset(new byteme::SerialBufferedWriter<unsigned char, std::shared_ptr<byteme::Writer> >(xptr, buffer_size));
    }

    // Checking that extract() and get/advance() play nice together.
    int b = 0;
    while (b < nbytes) {
        EXPECT_EQ(ptr->number(), b);
        auto remaining = std::min(extract_len, nbytes - b);
        ptr->write(contents.data() + b, remaining);
        b += remaining;

        EXPECT_EQ(ptr->number(), b);
        auto limit = b + std::min(extract_len, nbytes - b);
        for (; b < limit; ++b) {
            ptr->write(contents[b]);
        }
    }

    ptr->finish();
    EXPECT_EQ(xptr->get_output(), contents);
}

// We want to make sure we test cases where the extraction length is a factor of, greater than or smaller than the Writer buffer size;
// and similarly, the buffer size is a factor of, greater than or smaller than the total number of bytes.
// This gets some test coverage on the various scenarios in BufferedWriter::extract().
INSTANTIATE_TEST_SUITE_P(
    BufferedWriter,
    BufferedWriterArrayTest,
    ::testing::Combine(
        ::testing::Values(100, 256, 500, 1024), // number of bytes
        ::testing::Values(50, 128, 200, 512), // buffer size 
        ::testing::Values(25, 64, 128, 200), // extract len
        ::testing::Values(false, true) // parallel or not
    )
);

class BufferedWriterCharTest : public ::testing::TestWithParam<bool> {};

TEST_P(BufferedWriterCharTest, Basic) {
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
        byteme::RawBufferWriter writer({});

        // Checking it works with char types.
        std::unique_ptr<byteme::BufferedWriter<char> > ptr;
        if (parallel) {
            ptr.reset(new byteme::ParallelBufferedWriter<char, byteme::Writer*>(&writer, 10));
        } else {
            ptr.reset(new byteme::SerialBufferedWriter<char, byteme::Writer*>(&writer, 10));
        }

        if (mode == 0) {
            for (auto b : contents) { 
                ptr->write(b);
            }
        } else {
            const int nbytes = contents.size();
            for (int b = 0; b < nbytes; b += 11) {
                ptr->write(contents.c_str() + b, std::min(11, nbytes - b));
            }
        }

        ptr->flush();
        writer.finish();
        const auto& output = writer.get_output();
        std::string observed(output.begin(), output.end());
        EXPECT_EQ(observed, contents);
    }
}

INSTANTIATE_TEST_SUITE_P(
    BufferedWriter,
    BufferedWriterCharTest,
    ::testing::Values(false, true)
);

TEST(BufferedWriter, Empty) {
    byteme::RawBufferWriter writer({});
    std::string msg;
    try {
        byteme::SerialBufferedWriter<unsigned char, byteme::Writer*>(&writer, 0);
    } catch (std::exception& e) {
        msg = e.what();
    }
    EXPECT_TRUE(msg.find("positive") != std::string::npos);
}
