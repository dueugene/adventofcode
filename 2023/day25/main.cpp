#include <iostream>
#include <sstream>
#include <unordered_map>

#include "input_reader.hpp"

using namespace std;

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
    vector<string> input_data = input_reader::read_as_strings(filename);

    unordered_map<string, vector<string>> graph;
    for (const auto& line : input_data) {
        stringstream ss(line);
        string a;
        ss >> a;
        a = a.substr(0, a.size() - 1);
        string b;
        while (ss >> b) {
            graph[a].push_back(b);
            graph[b].push_back(a);
        }
    }
    cout << "nodes: " << graph.size() << endl;
    for (const auto& [k, v] : graph) {
        cout << k << ": ";
        for (const auto& s : v) {
            cout << s << " ";
        }
        cout << endl;
    }

    // did not get this one
    unsigned long long part1 = 0;
    unsigned long long part2 = 0;

    // part 1
    cout << "part1: " << part1 << endl;
    // part 2
    cout << "part2: " << part2 << endl;
    return 0;
}