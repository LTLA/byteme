#ifndef LINE_READER_H
#define LINE_READER_H

#include <string>
#include <vector>

struct LineReader {
    template<class B>
    void add(const B* buffer, size_t n) {
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

    bool continuing = false;
    std::vector<std::string> lines;
};

#endif
