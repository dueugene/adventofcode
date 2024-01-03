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

template <typename T>
using Matrix = vector<vector<T>>;

struct Range {
    Range (ulong beg, ulong range) {

    }
};

class Mapping {
public:
    Mapping() {

    }

    Mapping(string name, const Matrix<ulong>& input) : name_(name), mapping_(input) {
        auto comp = [](const vector<ulong>& a, const vector<ulong>& b) { return a[1] > b[1]; };
        sort(mapping_.begin(), mapping_.end(), comp);
    }
    ulong operator<<(ulong input) const {
        for (int i = 0; i < mapping_.size(); i++) {
            if (mapping_[i][1] <= input && mapping_[i][1] + mapping_[i][2] > input) {
                return mapping_[i][0] + input - mapping_[i][1];
            }
        }
        return input;
    }

private:
    string name_;
    Matrix<ulong> mapping_;
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
    string line;
    input.open(filename);
    vector<ulong> seeds;
    vector<Mapping> maps;
    while (getline(input, line)) {
        cout << line << endl;
        string temp;
        ulong num;
        if (seeds.empty()) {
            istringstream ss(line);
            ss >> temp;
            while (ss >> num) {
                seeds.push_back(num);
            }
            // empty line
            getline(input, line);
        } else {
            temp = line.substr(0, line.size() - 5);
            Matrix<ulong> a;
            while (getline(input, line) && !line.empty()) {
                cout << line << endl;
                istringstream ss(line);
                vector<ulong> b;
                while (ss >> num) {
                    b.push_back(num);
                }
                a.push_back(b);
            }
            maps.push_back(Mapping(temp, a));
        }
    }
    input.close();

    ulong part1 = 0;
    part1 = ~part1;
    for (ulong& seed : seeds) {
        ulong res = seed;
        for (const Mapping& map : maps) {
            res = map << res;
        }
        part1 = min(part1, res);
    }

    ulong part2 = 0;
    part2 = ~part2;
    for (int i = 0; i < seeds.size(); i = i+2) {
        // cout << i << " ";
        for (ulong seed = seeds[i]; seed < seeds[i] + seeds[i+1]; seed++) {
            // cout << seed << " ";
            ulong res = seed;
            for (const Mapping& map : maps) {
                res = map << res;
            }
            part2 = min(part2, res);
        }
    }

    // part 1
    cout << "part1: " << part1 << endl;
    // part 2
    cout << "part2: " << part2 << endl;
    return 0;
}