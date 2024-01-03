#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

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
    vector<string> lines;
    while (getline(input, line)) {
        line.erase(1,1);
        cout << line << endl;
        lines.push_back(line);
    }
    input.close();
    
    // part 1
    // A/X - rock
    // B/Y - paper
    // C/Z - scissors
    unordered_map<string, int> score_map;
    score_map["AX"] = 3 + 1;
    score_map["AY"] = 6 + 2;
    score_map["AZ"] = 0 + 3;

    score_map["BX"] = 0 + 1;
    score_map["BY"] = 3 + 2;
    score_map["BZ"] = 6 + 3;

    score_map["CX"] = 6 + 1;
    score_map["CY"] = 0 + 2;
    score_map["CZ"] = 3 + 3;

    int score = 0;
    for (string& s : lines) {
        score += score_map[s];
    }
    cout << "part1: " << score << endl;

    // part 2
    // A - rock
    // B - paper
    // C - scissors
    // X - lose
    // Y - draw
    // Z - win
    score_map["AX"] = 0 + 3;
    score_map["AY"] = 3 + 1;
    score_map["AZ"] = 6 + 2;

    score_map["BX"] = 0 + 1;
    score_map["BY"] = 3 + 2;
    score_map["BZ"] = 6 + 3;

    score_map["CX"] = 0 + 2;
    score_map["CY"] = 3 + 3;
    score_map["CZ"] = 6 + 1;
    score = 0;
    for (string& s : lines) {
        score += score_map[s];
    }
    cout << "part2: " << score << endl;
    return 0;
}