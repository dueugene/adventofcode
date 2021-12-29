#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>
#include <queue>
#include <unordered_map>

#include "../utils/file_read.cpp"

#include "packet.cpp"

using namespace std;

string hex_to_bin(string hex);

int main(int argc, char** argv) {
    // this will allow different input files to be passed
    string filename;
    if (argc > 1) {
        filename = argv[1];
    } else {
        // default input
        filename = "sample.txt";
    }

    // read the input file
    vector<string> hex_msgs = read_input(filename);

    // put into Packet format
    vector<Packet> packets;
    for (string msg : hex_msgs) {
        cout << msg << endl;
        string bin = hex_to_bin(msg);
        packets.push_back(Packet(bin));
        cout << bin << endl;
    }

    // part 1
    int result = 0;
    for (auto& p : packets) {
        result += p.sum_versions();
    }
    cout << "part1: " << result << endl;


    // part 2
    unsigned long long result2 = 0;
    for (auto& p : packets) {
        result2 += p.evaluate();
    }
    cout << "part2: " << result2 << endl;
    
    return 0;
}

string hex_to_bin(string hex) {
    unordered_map<char, string> mapping;
    mapping['0'] = "0000";
    mapping['1'] = "0001";
    mapping['2'] = "0010";
    mapping['3'] = "0011";
    mapping['4'] = "0100";
    mapping['5'] = "0101";
    mapping['6'] = "0110";
    mapping['7'] = "0111";
    mapping['8'] = "1000";
    mapping['9'] = "1001";
    mapping['A'] = "1010";
    mapping['B'] = "1011";
    mapping['C'] = "1100";
    mapping['D'] = "1101";
    mapping['E'] = "1110";
    mapping['F'] = "1111";
    string result;
    for (char c : hex) {
        result += mapping[c];
    }

    return result;
}