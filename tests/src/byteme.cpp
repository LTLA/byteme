#include <gtest/gtest.h>

#include "read_lines.h"

#include "byteme/temp_file_path.hpp"
#include "byteme/byteme.hpp"

#include "zlib.h"

TEST(UmbrellaTest, Basic) {
    std::vector<std::string> contents { "wakatte iru wa yo", "deai no shunkan ni", "ase ga nijinda Babyface" };
    std::string stuff;
    for (const auto& x : contents) {
        stuff += x + "\n";
    }

    auto gzname = byteme::temp_file_path("gzip");
    gzFile ohandle = gzopen(gzname.c_str(), "w");
    gzwrite(ohandle, stuff.c_str(), stuff.size());
    gzclose(ohandle);

    byteme::GzipFileReader reader(gzname.c_str(), 20);
    auto lines = read_lines(reader);
    EXPECT_EQ(lines, contents);
}
