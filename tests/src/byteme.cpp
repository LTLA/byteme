#include <gtest/gtest.h>

#include <cstdint>
#include <vector>

#include "utils.h"
#include "temp_file_path.h"

#include "byteme/byteme.hpp"
#include "zlib.h"

TEST(Miscellaneous, Umbrella) {
    auto contents = simulate_bytes(123, /* seed = */ 13);

    auto gzname = temp_file_path("gzip");
    gzFile ohandle = gzopen(gzname.c_str(), "w");
    gzwrite(ohandle, reinterpret_cast<const char*>(contents.data()), contents.size());
    gzclose(ohandle);

    byteme::GzipFileReader reader(gzname.c_str(), {});
    auto lines = full_read(reader, 10);
    EXPECT_EQ(lines, contents);
}

TEST(Miscellaneous, MagicNumbers) {
    unsigned char zlib_header[2] { 0x78, 0x01 };
    EXPECT_TRUE(byteme::is_zlib(zlib_header, 2));
    EXPECT_TRUE(byteme::is_zlib_or_gzip(zlib_header, 2));

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
    EXPECT_TRUE(byteme::is_zlib_or_gzip(gzip_header, 2));

    gzip_header[1] = 0x00;
    EXPECT_FALSE(byteme::is_gzip(gzip_header, 2));
    gzip_header[0] = 0x00;
    EXPECT_FALSE(byteme::is_gzip(gzip_header, 2));
    EXPECT_FALSE(byteme::is_zlib_or_gzip(zlib_header, 2));
}
