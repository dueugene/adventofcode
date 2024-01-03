#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <sstream>

#include "input_reader.hpp"

using namespace std;

enum colors {RED, BLUE, GREEN, NCOLORS};

unordered_map<string, colors> color_map {
    {"r", RED},
    {"b", BLUE},
    {"g", GREEN}
};

class Game {
public:
    Game(const string& input_str) {
        istringstream ss(input_str);
        string token;
        int num;
        char c;
        ss >> token >> id_ >> token;
        vector<int> temp(NCOLORS);
        while (ss >> num >> token) {
            temp[color_map[token.substr(0,1)]] = num;
            if (token[token.size()-1] != ',') {
                rounds_.push_back(temp);
                temp = vector<int>(NCOLORS);
            }
        }

    }

    int get_id() const {
        return id_;
    }
    bool game_possible(const vector<int>& cubes) const {
        for (const auto round : rounds_) {
            for (int col = 0; col < NCOLORS; col++) {
                if (round[col] > cubes[col]) {
                    return false;
                }
            }
        }
        return true;
    }

    int get_power() const {
        vector<int> best(NCOLORS);
        for (const auto round : rounds_) {
            for (int col = 0; col < NCOLORS; col++) {
                best[col] = max(best[col], round[col]);
            }
        }
        return best[RED] * best[BLUE] * best[GREEN];
    }
private:
    int id_;
    vector<vector<int>> rounds_;
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
    vector<string> lines = input_reader::read_as_strings(filename);
    vector<Game> games;
    for (auto line : lines) {
        games.push_back(Game(line));
    }
    
    int part1 = 0;
    int part2 = 0;
    vector<int> cubes(NCOLORS);
    cubes[RED] = 12;
    cubes[GREEN] = 13;
    cubes[BLUE] = 14;
    for (const auto& game : games) {
        if (game.game_possible(cubes)) {
            part1 += game.get_id();
        }
        cout << game.get_power() << endl;
        part2 += game.get_power();
    }
    // part 1
    cout << "part1: " << part1 << endl;
    // part 2
    cout << "part2: " << part2 << endl;
    return 0;
}