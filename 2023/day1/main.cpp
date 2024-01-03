#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>

#include "input_reader.hpp"

using namespace std;

const unordered_map<string, int> num_mapping {
    {"one", 1},
    {"two", 2},
    {"three", 3},
    {"four", 4},
    {"five", 5},
    {"six", 6},
    {"seven", 7},
    {"eight", 8},
    {"nine", 9}
};

int get_first_digit(const string& s, char dir = 'f') {
    if (dir == 'r') {
        for (int i = s.size(); i >= 0; i--) {
            if (s[i] >= '0' && s[i] <= '9') {
                return i;
            }
        }
        return -1;
    } else {
        for ( int i = 0; i < s.size(); i++) {
            if (s[i] >= '0' && s[i] <= '9') {
                return i;
            }
        }
        return s.size();
    }
    return -1;
}

pair<int, int> get_digits(const string& s, char dir = 'f') {
    if (dir == 'r') {
        int best = -1;
        int dig = 0;
        for (auto p : num_mapping) {
            int ind = s.rfind(p.first);
            if (ind != string::npos && ind > best) {
                best = ind;
                dig = p.second;
            }
        }
        int ind2 = get_first_digit(s, 'r');
        if (ind2 > best) {
            best = ind2;
            dig = s[best] - '0';
        }
        return {best, dig};

    } else {
        int best = s.size();
        int dig = 0;
        for (auto p : num_mapping) {
            int ind = s.find(p.first);
            if (ind != string::npos && ind < best) {
                best = ind;
                dig = p.second;
            }
        }
        int ind2 = get_first_digit(s);
        if (ind2 < best) {
            best = ind2;
            dig = s[best] - '0';
        }
        return {best, dig};
    }
    return {-1, -1};
}

int main(int argc, char** argv) {
    // this will allow different input files to be passed
    string filename;
    if (argc > 1) {
        filename = argv[1];
    } else {
        // default input
        filename = "input.txt";
    }

    vector<string> lines = input_reader::read_as_strings(filename);
    
    int part1 = 0;
    int part2 = 0;
    for (string& s : lines) {
        string num = "00";
        num[0] = s[get_first_digit(s)];
        num[1] = s[get_first_digit(s, 'r')];
        part1 += stoi(num);

        auto p1 = get_digits(s);
        auto p2 = get_digits(s, 'r');  
        part2 += p1.second*10 + p2.second;
    }
    // part 1
    cout << "part1: " << part1 << endl;
    // part 2
    cout << "part2: " << part2 << endl;
    return 0;
}