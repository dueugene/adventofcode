#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

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
    ifstream input;
    int curr = 0;
    string line;
    input.open(filename);
    priority_queue<int, vector<int>, greater<int>> top_three;
    while (getline(input, line)) {
        if (line.empty()) {
            top_three.push(curr);
            if (top_three.size() > 3) {
                top_three.pop();
            }
            curr = 0;
        } else {
            curr += stoi(line);
        }
    }
    top_three.push(curr);
    if (top_three.size() > 3) {
        top_three.pop();
    }
    input.close();
    
    int part1 = 0;
    int part2 = 0;
    while (!top_three.empty()) {
        part1 = top_three.top();
        part2 += part1;
        top_three.pop();
    }
    // part 1
    cout << "part1: " << part1 << endl;
    // part 2
    cout << "part2: " << part2 << endl;
    return 0;
}