#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>

// #include <boost/container_hash/hash.hpp>


using namespace std;

pair<int, int> string_to_num_pair(string str);

struct LineSeg {
    LineSeg(pair<int, int> a, pair<int, int> b)
     : start(a), end(b) {};
    pair<int, int> start;
    pair<int, int> end;

    void print() {
        cout << start.first << "," << start.second << " -> " << end.first << "," << end.second << endl;
    }
};

// copy from https://www.techiedelight.com/use-std-pair-key-std-unordered_map-cpp/
struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2> &pair) const {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
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
    input.open(filename);
    string line;
    vector<LineSeg> line_segs;
    while (getline(input, line)) {
        istringstream ss(line);
        string a, b, c;
        ss >> a >> b >> c;
        line_segs.push_back(LineSeg(string_to_num_pair(a), string_to_num_pair(c)));
    }
    input.close(); 

    for (auto item : line_segs) {
        item.print();
    }

    // part 1
    unordered_map<pair<int, int>, int, pair_hash> vals;
    vector<LineSeg> diag_lines;
    for (auto item : line_segs) {
        // assumes only vertical, horizontal lines
        pair<int, int> a = item.start;
        pair<int, int> b = item.end;
        if (a.second == b.second) {
            int start = min(a.first, b.first);
            int end = max(a.first, b.first);
            for (int i = start; i <= end; i++) {
                pair<int, int> co(i, a.second);
                vals[co]++;
            }
        } else if (a.first == b.first) {
            int start = min(a.second, b.second);
            int end = max(a.second, b.second);
            for (int j = start; j <= end; j++) {
                pair<int, int> co(a.first, j);
                vals[co]++;
            }
        } else {
            diag_lines.push_back(item);
        }
    }
    int count = 0;
    for (auto item : vals) {
        if (item.second >= 2) {
            count++;
        }
    }
    cout << "part1: " << count << endl;

    // part 2
    for (auto item : diag_lines) {
        pair<int, int> a = item.start;
        pair<int, int> b = item.end;
        if (a.first < b.first) {
            int j = a.second;
            int dir = a.second < b.second ? 1 : -1;
            for (int i = a.first; i <= b.first; i++) {
               pair<int, int> co(i, j); 
               vals[co]++;
               j += dir;
            }
        } else {
            int j = b.second;
            int dir = b.second < a.second ? 1 : -1;
            for (int i = b.first; i <= a.first; i++) {
               pair<int, int> co(i, j); 
               vals[co]++;
               j += dir;
            }
        }
    }
    count = 0;
    for (auto item : vals) {
        if (item.second >= 2) {
            count++;
        }
    }
    cout << "part2: " << count << endl;
    return 0;
}


pair<int, int> string_to_num_pair(string str) {
    istringstream ss(str);
    int a, b;
    char c;
    ss >> a >> c >> b;
    return pair<int, int> {a, b};
}