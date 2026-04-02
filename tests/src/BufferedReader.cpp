#include <gtest/gtest.h>

#include "temp_file_path.h"
#include "utils.h"

#include "byteme/BufferedReader.hpp"
#include "byteme/RawBufferReader.hpp"

#include <fstream>
#include <memory>

class BufferedReaderGetTest : public ::testing::TestWithParam<std::tuple<int, int, bool> > {};

TEST_P(BufferedReaderGetTest, Basic) {
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
    BufferedReader,
    BufferedReaderGetTest,
    ::testing::Combine(
        ::testing::Values(100, 256, 500, 1024), // number of bytes
        ::testing::Values(50, 128, 200, 512), // buffer size
        ::testing::Values(false, true) // parallel or not
    )
);

class BufferedReaderExtractSimpleTest : public ::testing::TestWithParam<std::tuple<bool, bool> > {};

TEST_P(BufferedReaderExtractSimpleTest, Basic) {
    auto contents = simulate_bytes(100, /* seed = */ 111);
    auto params = GetParam();
    bool use_until = std::get<0>(params);
    bool parallel = std::get<1>(params);

    auto create_buffered_reader = [&](byteme::Reader& reader, std::size_t buffer_size) -> std::unique_ptr<byteme::BufferedReader<unsigned char> > {
        if (parallel) {
            return std::make_unique<byteme::ParallelBufferedReader<unsigned char, byteme::Reader*> >(&reader, buffer_size);
        } else {
            return std::make_unique<byteme::SerialBufferedReader<unsigned char, byteme::Reader*> >(&reader, buffer_size);
        }
    };

    // Case A: extracting from the buffer, without any refills.
    {
        byteme::RawBufferReader reader(contents.data(), contents.size());
        auto bufreader = create_buffered_reader(reader, 100);

        std::vector<unsigned char> extracted(40); 
        if (use_until) {
            auto out = bufreader->extract_until(extracted.size(), extracted.data());
            EXPECT_EQ(out, extracted.size());
            EXPECT_EQ(bufreader->position(), 39);
            EXPECT_EQ(bufreader->get(), contents[39]);
        } else {
            auto out = bufreader->extract(extracted.size(), extracted.data());
            EXPECT_EQ(out.first, extracted.size());
            EXPECT_TRUE(out.second);
            EXPECT_EQ(bufreader->position(), 40);
        }

        auto truncated = contents;
        truncated.resize(extracted.size());
        EXPECT_EQ(truncated, extracted);
    }

    // Case B: extracting from the buffer after the last refill of the stream.
    {
        byteme::RawBufferReader reader(contents.data(), contents.size());
        auto bufreader = create_buffered_reader(reader, 40);

        std::vector<unsigned char> collected;
        std::vector<unsigned char> extracted(90); 

        auto out = bufreader->extract(extracted.size(), extracted.data()); // case D exit, just to prime the state for the actual case B exit.
        EXPECT_EQ(out.first, extracted.size());
        EXPECT_TRUE(out.second);
        collected.insert(collected.end(), extracted.begin(), extracted.end());
        EXPECT_EQ(bufreader->position(), 90);

        if (use_until) {
            auto out = bufreader->extract_until(extracted.size(), extracted.data());
            EXPECT_EQ(out, 10);
            collected.insert(collected.end(), extracted.begin(), extracted.begin() + out);
            EXPECT_EQ(bufreader->get(), contents.back());
            EXPECT_EQ(bufreader->position(), 99);
            EXPECT_FALSE(bufreader->advance());
            EXPECT_EQ(bufreader->position(), 100);
        } else {
            auto out = bufreader->extract(extracted.size(), extracted.data());
            EXPECT_EQ(out.first, 10);
            EXPECT_FALSE(out.second);
            collected.insert(collected.end(), extracted.begin(), extracted.begin() + out.first);
            EXPECT_EQ(bufreader->position(), 100);
        }

        EXPECT_EQ(collected, contents);
    }

    // Case C: directly reading from the Reader into the output array, and terminating inside the loop when there aren't any bytes left.
    {
        byteme::RawBufferReader reader(contents.data(), contents.size());
        auto bufreader = create_buffered_reader(reader, 20);

        std::vector<unsigned char> extracted(1000); 

        if (use_until) {
            auto out = bufreader->extract_until(extracted.size(), extracted.data());
            EXPECT_EQ(out, 100);
            EXPECT_EQ(bufreader->get(), contents.back());
            EXPECT_EQ(bufreader->position(), 99);
            EXPECT_FALSE(bufreader->advance());
            EXPECT_EQ(bufreader->position(), 100);
        } else {
            auto out = bufreader->extract(extracted.size(), extracted.data());
            EXPECT_EQ(out.first, 100);
            EXPECT_FALSE(out.second);
        }

        extracted.resize(100);
        EXPECT_EQ(extracted, contents);
    }

    // Case D: some direct reads into the output array, and then an extra read of a few more bytes from the buffer.
    {
        byteme::RawBufferReader reader(contents.data(), contents.size());
        auto bufreader = create_buffered_reader(reader, 25);

        std::vector<unsigned char> extracted(80); 

        if (use_until) {
            auto out = bufreader->extract_until(extracted.size(), extracted.data());
            EXPECT_EQ(out, 80);
            EXPECT_EQ(bufreader->get(), contents[79]);
            EXPECT_EQ(bufreader->position(), 79);
            EXPECT_TRUE(bufreader->advance());
        } else {
            auto out = bufreader->extract(extracted.size(), extracted.data());
            EXPECT_EQ(out.first, 80);
            EXPECT_TRUE(out.second);
        }

        auto truncated = contents;
        truncated.resize(80);
        extracted.resize(80);
        EXPECT_EQ(extracted, truncated);
    }

    // Case D (extract) or E (extract_until): some direct reads into the output array, and then an extra read of all remaining bytes from the buffer until exhaustion.
    {
        byteme::RawBufferReader reader(contents.data(), contents.size());
        auto bufreader = create_buffered_reader(reader, 25);

        // +1, to test the last failed refill() when the extraction length is not a multiple of the buffer size.
        std::vector<unsigned char> extracted(101); 

        if (use_until) {
            auto out = bufreader->extract_until(extracted.size(), extracted.data());
            EXPECT_EQ(out, 100);
            EXPECT_EQ(bufreader->get(), contents.back());
            EXPECT_EQ(bufreader->position(), 99);
            EXPECT_FALSE(bufreader->advance());
        } else {
            auto out = bufreader->extract(extracted.size(), extracted.data());
            EXPECT_EQ(out.first, 100);
            EXPECT_FALSE(out.second);
        }

        extracted.resize(contents.size());
        EXPECT_EQ(extracted, contents);
    }
}

INSTANTIATE_TEST_SUITE_P(
    BufferedReader,
    BufferedReaderExtractSimpleTest,
    ::testing::Combine(
        ::testing::Values(false, true),
        ::testing::Values(false, true)
    )
);

class BufferedReaderExtractTest : public ::testing::TestWithParam<std::tuple<int, int, int, bool> > {};

TEST_P(BufferedReaderExtractTest, Basic) {
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
    std::vector<unsigned char> extracted(extract_len);
    while (1) {
        auto out = ptr->extract(extracted.size(), extracted.data());
        observed.insert(observed.end(), extracted.data(), extracted.data() + out.first);
        EXPECT_EQ(ptr->position(), observed.size());
        EXPECT_EQ(out.second, ptr->valid());
        if (!out.second) {
            break;
        }
    }

    EXPECT_EQ(observed, contents);
}

TEST_P(BufferedReaderExtractTest, Until) {
    auto param = GetParam();
    auto nbytes = std::get<0>(param);
    auto buffer_size = std::get<1>(param);
    auto extract_len = std::get<2>(param);
    auto parallel = std::get<3>(param);

    auto contents = simulate_bytes(nbytes, /* seed = */ 67 * nbytes + buffer_size + extract_len + parallel);
    auto xptr = std::make_unique<byteme::RawBufferReader>(contents.data(), contents.size()); // Using smart pointers for some variety.

    std::unique_ptr<byteme::BufferedReader<unsigned char> > ptr;
    if (parallel) {
        ptr.reset(new byteme::ParallelBufferedReader<unsigned char, std::unique_ptr<byteme::Reader> >(std::move(xptr), buffer_size));
    } else {
        ptr.reset(new byteme::SerialBufferedReader<unsigned char, std::unique_ptr<byteme::Reader> >(std::move(xptr), buffer_size));
    }

    std::vector<unsigned char> observed;
    std::vector<unsigned char> extracted(extract_len);
    while (1) {
        auto out = ptr->extract_until(extracted.size(), extracted.data());
        observed.insert(observed.end(), extracted.data(), extracted.data() + out);
        EXPECT_TRUE(ptr->valid());
        EXPECT_EQ(ptr->get(), observed.back()) << ptr->position(); 
        EXPECT_EQ(ptr->position(), observed.size() - 1);
        if (out < extracted.size()) {
            EXPECT_FALSE(ptr->advance());
            break;
        }
        if (!ptr->advance()) {
            break;
        }
    }

    EXPECT_EQ(observed, contents);
}

TEST_P(BufferedReaderExtractTest, Mixed) {
    auto param = GetParam();
    auto nbytes = std::get<0>(param);
    auto buffer_size = std::get<1>(param);
    auto extract_len = std::get<2>(param);
    auto parallel = std::get<3>(param);

    auto contents = simulate_bytes(nbytes, /* seed = */ 69 * nbytes + buffer_size + extract_len + parallel);
    auto xptr = std::make_unique<byteme::RawBufferReader>(contents.data(), contents.size()); // Using smart pointers for some variety.

    std::unique_ptr<byteme::BufferedReader<unsigned char> > ptr;
    if (parallel) {
        ptr.reset(new byteme::ParallelBufferedReader<unsigned char, std::unique_ptr<byteme::Reader> >(std::move(xptr), buffer_size));
    } else {
        ptr.reset(new byteme::SerialBufferedReader<unsigned char, std::unique_ptr<byteme::Reader> >(std::move(xptr), buffer_size));
    }

    // Checking that extract() and get/advance() play nice together.
    std::vector<unsigned char> observed;
    std::vector<unsigned char> extracted(extract_len);
    while (1) {
        auto out = ptr->extract(extracted.size(), extracted.data());
        observed.insert(observed.end(), extracted.data(), extracted.data() + out.first);
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
    BufferedReader,
    BufferedReaderExtractTest,
    ::testing::Combine(
        ::testing::Values(100, 256, 500, 1024), // number of bytes
        ::testing::Values(50, 128, 200, 512), // buffer size 
        ::testing::Values(25, 64, 128, 200), // extract len
        ::testing::Values(false, true) // parallel or not
    )
);

class BufferedReaderCharTest : public ::testing::TestWithParam<bool> {};

TEST_P(BufferedReaderCharTest, Basic) {
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
    BufferedReader,
    BufferedReaderCharTest,
    ::testing::Values(false, true)
);

class FailedReader : public byteme::Reader {
public:
    FailedReader(std::size_t fail) : my_fail(fail) {}

    std::size_t read(unsigned char* ptr, std::size_t n) {
        if (my_fail - position >= n) {
            std::fill_n(ptr, n, 1);
            position += n;
        } else {
            throw std::runtime_error("OOPS");
        }
        return n;
    }

private:
    std::size_t position = 0;
    std::size_t my_fail;
};

TEST(BufferedReader, ParallelErrors) {
    std::cout << "OKAY" << std::endl;
    // Check that exceptions are properly handled in the constructor.
    {
        FailedReader reader(100);
        std::string msg;
        try {
            byteme::ParallelBufferedReader<unsigned char, byteme::Reader*>(&reader, 300);
        } catch (std::exception& e) {
            msg = e.what();
        }
        EXPECT_EQ(msg, "OOPS");
    }

    std::cout << "OKAY" << std::endl;
    // Check that exceptions are properly handled for `refill()` with no arguments.
    {
        FailedReader reader(100);
        byteme::ParallelBufferedReader<unsigned char, byteme::Reader*> bufreader(&reader, 50);

        std::string msg;
        try {
            while (1) {
                bufreader.advance(); 
            }
        } catch (std::exception& e) {
            msg = e.what();
        }
        EXPECT_EQ(msg, "OOPS");
    }

    std::cout << "OKAY" << std::endl;
    // Check that exceptions are properly handled for `refill()` with some arguments.
    {
        FailedReader reader(100);
        byteme::ParallelBufferedReader<unsigned char, byteme::Reader*> bufreader(&reader, 50);

        std::string msg;
        std::vector<unsigned char> buffer(1000);
        try {
            bufreader.extract(buffer.size(), buffer.data());
        } catch (std::exception& e) {
            msg = e.what();
        }
        EXPECT_EQ(msg, "OOPS");
    }
    std::cout << "OKAY" << std::endl;
}
