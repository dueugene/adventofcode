#include <iostream>
#include <vector>
#include <sstream>
#include <numeric>

#include "input_reader.hpp"

using namespace std;

template <typename T>
using Matrix = vector<vector<T>>;
using ulong = unsigned long long;

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
    vector<string> lines = input_reader::read_as_strings(filename);

    Matrix<int> data;
    vector<string> big_race;
    for (auto line : lines) {
        cout << line << endl;
        string temp;
        string a;
        int num;
        istringstream ss(line);
        vector<int> nums;
        ss >> temp;
        while (ss >> num) {
            nums.push_back(num);
            a += to_string(num);
        }
        data.push_back(nums);
        big_race.push_back(a);
    }
    
    ulong part1 = 0;
    vector<ulong> result;
    for (int j = 0; j < data[0].size(); j++) {
        ulong wins = 0;
        for (int i = 0; i < data[0][j]; i++) {
            int dist = (data[0][j] - i)*i;
            if (dist > data[1][j]) {
                wins++;
            }
        }
        result.push_back(wins);
    }
    part1 = accumulate(begin(result), end(result), 1, multiplies<ulong>());

    ulong part2 = 0;
    for (ulong i = 0; i < stoull(big_race[0]); i++) {
        ulong dist = (stoull(big_race[0]) - i)*i;
        if (dist > stoull(big_race[1])) {
            part2++;
        }
    }


    // part 1
    cout << "part1: " << part1 << endl;
    // part 2
    cout << "part2: " << part2 << endl;
    return 0;
}