#include <iostream>
#include <fstream>
#include <vector>
#include <string>

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
    vector<string> commands;
    vector<int> vals;
    string command;
    int val;
    input.open(filename);
    while (input >> command >> val) {
        commands.push_back(command);
        vals.push_back(val);
    }
    input.close();

    // part 1
    int position = 0;
    int depth = 0;
    for (int i = 0; i < commands.size(); i++) {
        string c = commands[i];
        if (c == "forward") {
            position += vals[i];
        } else if (c == "up") {
            depth -= vals[i];
        } else if (c == "down") {
            depth += vals[i];
        } else {
            // uh oh
        }
    }
    cout << "position: " << position << ", depth: " << depth << endl;

    // part 2
    position = 0;
    depth = 0;
    int aim = 0;
    for (int i = 0; i < commands.size(); i++) {
        string c = commands[i];
        if (c == "forward") {
            position += vals[i];
            depth += aim * vals[i];
        } else if (c == "up") {
            aim -= vals[i];
        } else if (c == "down") {
            aim += vals[i];
        } else {
            // uh oh
        }
    }
    cout << "position: " << position << ", depth: " << depth << endl;
    return 0;
}