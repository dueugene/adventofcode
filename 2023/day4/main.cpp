#include <iostream>
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
    vector<string> lines = input_reader::read_as_strings(filename);
    vector<vector<int>> winning_numbers;
    vector<vector<int>> my_numbers;
    for (auto line : lines) {
        cout << line << endl;
        istringstream ss(line);
        string temp;
        int num;
        vector<int> num_buffer;
        ss >> temp >> num >> temp;
        while (ss >> temp) {
            if (temp[0] == '|') {
                sort(num_buffer.begin(), num_buffer.end());
                winning_numbers.push_back(num_buffer);
                num_buffer.clear();
            } else {
                num_buffer.push_back(stoi(temp));
            }
        }
        sort(num_buffer.begin(), num_buffer.end());
        my_numbers.push_back(num_buffer);
    }
    
    int part1 = 0;
    int part2 = 0;

    vector<int> scores;
    for (int i = 0; i < my_numbers.size(); i++) {
        int score = 0;
        for (int j = 0; j < my_numbers[i].size(); j++) {
            if (binary_search(winning_numbers[i].begin(), winning_numbers[i].end(), my_numbers[i][j])) {
                score++;
            }
        }
        part1 += score ? pow(2, score-1) : 0;
        scores.push_back(score);
    }

    vector<int> cards(scores.size(), 1);
    for (int i = 0; i < cards.size(); i++) {
        for (int j = i + 1; j <= i + scores[i]; j++) {
            cards[j] += cards[i];
        }
    }
    part2 = accumulate(cards.begin(), cards.end(), 0);

    // part 1
    cout << "part1: " << part1 << endl;
    // part 2
    cout << "part2: " << part2 << endl;
    return 0;
}