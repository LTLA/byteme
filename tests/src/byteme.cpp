#include <gtest/gtest.h>

#include <cstdint>
#include <vector>

#include "read_lines.h"
#include "temp_file_path.h"

#include "byteme/byteme.hpp"
#include "zlib.h"

TEST(Miscellaneous, Umbrella) {
    std::vector<std::string> contents { "wakatte iru wa yo", "deai no shunkan ni", "ase ga nijinda Babyface" };
    std::string stuff;
    for (const auto& x : contents) {
        stuff += x + "\n";
    }

    auto gzname = temp_file_path("gzip");
    gzFile ohandle = gzopen(gzname.c_str(), "w");
    gzwrite(ohandle, stuff.c_str(), stuff.size());
    gzclose(ohandle);

    byteme::GzipFileReader reader(gzname.c_str(), {});
    auto lines = read_lines(reader);
    EXPECT_EQ(lines, contents);
}

TEST(Miscellaneous, CheckBufferSize) {
    EXPECT_FALSE(byteme::exceeds_cap<std::int8_t>(10));
    EXPECT_TRUE(byteme::exceeds_cap<std::int8_t>(1000));
    EXPECT_FALSE(byteme::exceeds_cap<std::size_t>(256));

    EXPECT_EQ(byteme::check_buffer_size<std::uint8_t>(10), 10);
    EXPECT_EQ(byteme::check_buffer_size<std::uint8_t>(1000), 255);
    EXPECT_EQ(byteme::check_buffer_size<std::int16_t>(1000), 1000);
    EXPECT_EQ(byteme::check_buffer_size<std::int16_t>(40000), 32767);
    EXPECT_EQ(byteme::check_buffer_size<std::uint16_t>(40000), 40000);

    EXPECT_EQ(byteme::check_buffer_size(10), 10);
    EXPECT_EQ(byteme::check_buffer_size(10000), 10000);

    EXPECT_EQ(byteme::cap<std::size_t>(256), 256);
    EXPECT_EQ(byteme::cap<std::size_t>(1000), 1000);
    EXPECT_EQ(byteme::cap<std::uint8_t>(256), 255);

    std::vector<unsigned char> buffers(1000);
    int counter = 0;
    for (auto& b : buffers) {
        b = counter % std::numeric_limits<unsigned char>::max();
        ++counter;
    }

    {
        std::vector<unsigned char> reference;
        byteme::safe_write<uint8_t, false>(
            buffers.data(),
            static_cast<int>(buffers.size()),
            [&](const unsigned char* b, int n) -> void {
                reference.insert(reference.end(), b, b + n);
            }
        );
        EXPECT_EQ(buffers, reference);
    }

    for (int i = 0; i < 2; ++i) {
        std::vector<unsigned char> reference;
        byteme::safe_write<uint8_t, true>(
            buffers.data(),
            (i ? 0 : static_cast<unsigned>(buffers.size())),
            [&](const unsigned char* b, unsigned n) -> void {
                reference.insert(reference.end(), b, b + n);
            }
        );
        if (i) {
            EXPECT_TRUE(reference.empty());
        } else {
            EXPECT_EQ(buffers, reference);
        }
    }
}

TEST(Miscellaneous, MagicNumbers) {
    unsigned char zlib_header[2] { 0x78, 0x01 };
    EXPECT_TRUE(byteme::is_zlib(zlib_header, 2));
    zlib_header[1] = 0x5e;
    EXPECT_TRUE(byteme::is_zlib(zlib_header, 2));
    zlib_header[1] = 0x9c;
    EXPECT_TRUE(byteme::is_zlib(zlib_header, 2));
    zlib_header[1] = 0xda;
    EXPECT_TRUE(byteme::is_zlib(zlib_header, 2));
    zlib_header[1] = 0x20;
    EXPECT_TRUE(byteme::is_zlib(zlib_header, 2));
    zlib_header[1] = 0x7d;
    EXPECT_TRUE(byteme::is_zlib(zlib_header, 2));
    zlib_header[1] = 0xbb;
    EXPECT_TRUE(byteme::is_zlib(zlib_header, 2));
    zlib_header[1] = 0xf9;
    EXPECT_TRUE(byteme::is_zlib(zlib_header, 2));
    EXPECT_FALSE(byteme::is_zlib(zlib_header, 1));
    zlib_header[1] = 0x00;
    EXPECT_FALSE(byteme::is_zlib(zlib_header, 2));
    zlib_header[0] = 0x00;
    EXPECT_FALSE(byteme::is_zlib(zlib_header, 2));

    unsigned char gzip_header[2] { 0x1f, 0x8b };
    EXPECT_FALSE(byteme::is_gzip(gzip_header, 1));
    EXPECT_TRUE(byteme::is_gzip(gzip_header, 2));
    gzip_header[1] = 0x00;
    EXPECT_FALSE(byteme::is_gzip(gzip_header, 2));
    gzip_header[0] = 0x00;
    EXPECT_FALSE(byteme::is_gzip(gzip_header, 2));
}
