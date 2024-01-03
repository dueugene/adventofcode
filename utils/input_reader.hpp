#include <fstream>
#include <string>
#include <vector>

namespace input_reader {
std::vector<std::string> read_as_strings(std::string filename) {
    std::ifstream input;
    input.open(filename);
    std::string line;
    std::vector<std::string> result;
    while (getline(input, line)) {
        result.push_back(line);
    }
    input.close();
    
    return result;
}

std::vector<std::vector<char>> read_as_matrix(std::string filename) {
    std::ifstream input;
    input.open(filename);
    std::string line;
    std::vector<std::vector<char>> result;
    while (getline(input, line)) {
        std::vector<char> line_as_char(line.begin(), line.end());
        result.push_back(line_as_char);
    }
    input.close();

    return result;
}

std::string read_single_line(std::string filename) {
    std::ifstream input;
    input.open(filename);
    std::string line;
    getline(input, line);
    input.close();

    return line;
}
} // namespace input_reader