#include <iostream>
#include <vector>
#include <cstring>
#include <queue>

size_t FindNewLine(const char* part, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        if (part[i] == '\n') {
            return i;
        }
    }
    return len;
}

int Parse(const std::string& pattern) {
    const size_t len = pattern.length();
    std::vector<char> buf_one(len), buf_two(len);

    bool got_stuff = (bool) std::cin.read(buf_two.data(), buf_two.size());
    for (size_t i = 0; i < len; ++i) {
        if (buf_two[i] == '\n') {
            return 1;
        }
        std::cout << buf_two[i];
    }
    while (got_stuff) {
        // Get two pieces of input and merge them into curr_piece
        std::swap(buf_one, buf_two);
        got_stuff = (bool) std::cin.read(buf_two.data(), buf_two.size());
        if (buf_two[len-1] == '\n') {
            got_stuff = false;
        }
        size_t buf_one_n = FindNewLine(buf_one.data(), len);
        size_t buf_two_n = FindNewLine(buf_two.data(), len);
        std::vector<char> current_piece_buf(buf_one_n + buf_two_n + 1);
        std::memcpy(current_piece_buf.data(), buf_one.data(), buf_one_n);
        std::memcpy(current_piece_buf.data() + buf_one_n, buf_two.data(), buf_two_n);
        current_piece_buf[current_piece_buf.size()-1] = '\0';
        std::string curr_piece(current_piece_buf.data());

        // Find all pattern occurances in the curr_piece
        size_t found = curr_piece.find(pattern);
        std::queue<size_t> positions;
        while (found != std::string::npos && found < len) {
            positions.push(found+len);
            // std::cout << " " << found;
            found = curr_piece.find(pattern, found+1);
        }

        // Print piece of result
        for (size_t i = len; i < curr_piece.length(); ++i) {
            if (!positions.empty() && i == positions.front()) {
                std::cout << '*';
                positions.pop();
            }
            std::cout << curr_piece[i];
        }
        if (!positions.empty()) {
            positions.pop();
            std::cout << '*';
        }
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        return 1;
    } else {
        return Parse(argv[1]);
    }
}
