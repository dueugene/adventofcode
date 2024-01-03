#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <sstream>
#include <list>
#include <algorithm>
#include <cmath>
#include <numeric>

using namespace std;


ulong calc_finish(const unordered_map<string, pair<string, string>>& mapping, const string& sequence, string curr) {
    ulong count = 0;
    int ind = 0;
     while (curr[2] != 'Z') {
        char c = sequence[ind];
        if (c == 'L') {
            //cout <<"L: " << curr << " " << mapping[curr].first << endl;
            curr = mapping.at(curr).first;
        } else {
            //cout <<"R: " << curr << " " << mapping[curr].second << endl;
            curr = mapping.at(curr).second;
        }
        count++;
        ind++;
        ind %= sequence.size();
    }
    return count;
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

    // read the input file
    ifstream input;
    input.open(filename);
    unordered_map<string, pair<string, string>> mapping;
    auto custom_hash = [](const string& a) {
        return std::hash<char>{}(a[2]);
    };
    auto custom_equal = [](const string& a, const string& b) {
        return a[2] == b[2];
    };

    unordered_map<string, vector<pair<string, string>>, decltype(custom_hash), decltype(custom_equal)> mapping2(10, custom_hash, custom_equal);
    string sequence;
    vector<string> starting_a;
    string a;
    int num;
    while (getline(input, a)) {
        if (sequence.empty()) {
            sequence = a;
            getline(input, a);
        } else {
            string k = a.substr(0, 3);
            string b = a.substr(7 , 3);
            string c = a.substr(12 , 3);
            mapping[k] = {b, c};
            mapping2[k].push_back({b, c});
            if (a[2] == 'A') {
                starting_a.push_back(k);
            }
        }
    }
    input.close();
    for (auto p : mapping) {
        cout << p.first << " " << p.second.first << " " << p.second.second << endl;
    }
    for (auto p : mapping2) {
        cout << p.first << ": ";
        for (auto k : p.second) {
            cout << k.first << "," << k.second << " ";
        }
        cout << endl;
    }

    ulong part1 = 0;
    string curr = "AAA";
    int ind = 0;
    while (curr != "ZZZ") {
        char c = sequence[ind];
        if (c == 'L') {
            cout <<"L: " << curr << " " << mapping[curr].first << endl;
            curr = mapping[curr].first;
        } else {
            cout <<"R: " << curr << " " << mapping[curr].second << endl;
            curr = mapping[curr].second;
        }
        part1++;
        ind++;
        ind %= sequence.size();
    }
    ulong part2 = 0;
    for (auto p : starting_a) {
        cout << p << ": " << calc_finish(mapping, sequence, p) << endl;
    }


    // part 1
    cout << "part1: " << part1 << endl;
    // part 2
    cout << "part2: " << part2 << endl;
    return 0;
}