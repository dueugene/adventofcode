#include <fstream>
#include <string>
#include <vector>

std::vector<std::string> read_input(std::string filename) {
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