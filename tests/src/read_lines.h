#ifndef READ_LINES_H 
#define READ_LINES_H 

#include <string>
#include <vector>

template<class Reader>
std::vector<std::string> read_lines(Reader& reader, std::size_t chunk) { 
    std::vector<std::string> lines;
    bool continuing = false;

    std::vector<unsigned char> storage(chunk);
    while (1) {
        const auto n = reader.read(storage.data(), chunk);
        const auto buffer = reinterpret_cast<char*>(storage.data());

        std::size_t pos = 0;
        std::size_t last = 0;
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

        if (n < chunk) {
            break;
        }
    } 

    return lines;
}

#endif
