#include <iostream>
#include <sstream>
#include <vector>

#include "input_reader.hpp"

using namespace std;


long long int extrapolate(vector<long long int>& input) {
    // for (auto p : input) {
    //     cout << p << " ";
    // }
    // cout << endl;
    if (input.size() == 1) {return input[0];}
    vector<long long int> temp(input.size()-1);
    bool recurse = false;
    for (int i = 0; i < temp.size(); i++) {
        temp[i] = input[i+1] - input[i];
        if (temp[i] != 0) {
            recurse = true;
        }
    }
    if (recurse) {
        auto result = extrapolate(temp) + input[input.size()- 1];
        return result;
    }
    return input[input.size()- 1];
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
    vector<string> lines = input_reader::read_as_strings(filename);
    vector<vector<long long int>> input_data;
    long long int num;
    for (auto line : lines) {
        istringstream ss(line);
        vector<long long int> b;
        while(ss >> num) {
            b.push_back(num);
        }
        input_data.push_back(b);
    }


    long long int part1 = 0;
    for (auto& l : input_data) {
        part1 += extrapolate(l);
    }
    long long int part2 = 0;
    for (auto& l : input_data) {
        reverse(l.begin(), l.end());
        part2 += extrapolate(l);
    }

    // part 1
    cout << "part1: " << part1 << endl;
    // part 2
    cout << "part2: " << part2 << endl;
    return 0;
}