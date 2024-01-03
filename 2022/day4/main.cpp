#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <set>
#include <bitset>

using namespace std;

class Elf {
public:
    Elf(const string& input) {
        int ind = 0;
        int a = to_int(input, ind);
        ind++;
        int b = to_int(input, ind);
        ind++;
        first = {a, b};
        a = to_int(input, ind);
        ind++;
        b = to_int(input, ind);
        second = {a, b};
    }
    bool is_overlapped() {
        if (first.first >= second.first && first.second <= second.second) {
            return true;
        }
        if (second.first >= first.first && second.second <= first.second) {
            return true;
        }
        return false;
    }

    bool partially_overlapped() {
        if (first.second <= second.second) {
            return second.first <= first.second;
        }
        return first.first <= second.second;
    }
private:
    int to_int(const string& input, int& ind) {
        int result = 0;
        while (ind < input.size()) {
            char c = input.at(ind);
            if (c < '0' || c > '9') {
                break;
            }
            ind++;
            result *= 10;
            result += c - '0';
        }
        return result;
    }
    pair<int, int> first;
    pair<int, int> second;
};

int main(int argc, char** argv) {
    // this will allow different input files to be passed
    string filename;
    if (argc > 1) {
        filename = argv[1];
    } else {
        // default input
        filename = "input.txt";
    }

    // read the input file
    ifstream input;
    int curr = 0;
    string line;
    input.open(filename);
    vector<Elf> elfs;
    while (getline(input, line)) {
        cout << line << endl;
        elfs.push_back(Elf(line));
    }
    input.close();
    
    // part 1
    int score = 0;
    for (auto& elf : elfs) {
        if (elf.is_overlapped()) {
            score++;
        }
    }
    cout << "part1: " << score << endl;

    // part 2
    score = 0;
    for (auto& elf : elfs) {
        if (elf.partially_overlapped()) {
            score++;
        }
    }
    cout << "part2: " << score << endl;
    return 0;
}