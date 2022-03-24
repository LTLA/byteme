#ifndef READ_LINES_H 
#define READ_LINES_H 

#include <string>
#include <vector>

template<class Reader>
std::vector<std::string> read_lines(Reader& reader) { 
    std::vector<std::string> lines;
    bool remaining = true;
    bool continuing = false;

    while (remaining) {
        remaining = reader();
        const char* buffer = reinterpret_cast<const char*>(reader.buffer()); // reinterpreting as chars.
        size_t n = reader.available();

        size_t pos = 0;
        size_t last = 0;

        while (1) {
            while (pos < n && buffer[pos] != '\n') {
                ++pos;
            }

            if (pos == n) {
                if (continuing) {
                    lines.back() += std::string(buffer + last, buffer + pos);
                } else if (pos > last) { // don't add zero-length strings at this point.
                    lines.emplace_back(buffer + last, buffer + pos);
                    continuing = true;
                }
                break;
            }

            // Must be at a newline, if the loop didn't stop because of pos < n.
            if (continuing) {
                lines.back() += std::string(buffer + last, buffer + pos);
                continuing = false;
            } else {
                lines.emplace_back(buffer + last, buffer + pos);
            }

            ++pos;
            last = pos;
        }
    }

    return lines;
}

#endif
