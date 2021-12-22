#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <list>

#include "bingo.cpp"

using namespace std;

vector<int> line_to_vector(string line, char delim);

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
    // bingo numbers
    string line;
    getline(input, line);
    vector<int> nums = line_to_vector(line, ',');
    // bingo boards
    list<Bingo> bingo_boards;
    while (getline(input,line)) {
        if (line.empty()) {

        } else {
            vector<vector<int>> board;
            for (int i = 0; i < 5; i++) {
                board.push_back(line_to_vector(line, ' '));
                getline(input,line);
            }
            bingo_boards.push_back(Bingo(board));
        }
    }
    input.close();

    // part 1 & 2
    int count = 0;
    cout << bingo_boards.size() << endl;
    for (int num : nums) {
        // cout << num << endl;
        list<list<Bingo>::iterator> to_delete;
        for (auto it = bingo_boards.begin(); it != bingo_boards.end(); it++) {
            Bingo& board = *it;
            if (board.call_number(num)) {
                cout << ++count << ": board score: " <<  board.get_score() << endl;
                to_delete.push_back(it);
            }
        }
        for (auto item : to_delete) {
            bingo_boards.erase(item);
        }
    }

    return 0;
}

vector<int> line_to_vector(string line, char delim) {
    vector<int> nums;
    int num;
    istringstream ss(line);
    if (delim == ' ') {
        while (ss >> num) {
            nums.push_back(num);
        }
    } else {
        while (ss >> num >> delim) {
            nums.push_back(num);
        }
    }

    return nums;
}