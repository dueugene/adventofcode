#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>
#include <queue>
#include <unordered_set>

using namespace std;

int simulate_octopuses(vector<string>& grid);
void update_grid(vector<string>& grid, int i, int j, vector<vector<bool>>& flashed);

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
    vector<string> grid;
    while (getline(input, line)) {
        cout << line << endl;
        grid.push_back(line);
    }
    input.close();


    // part 1
    vector<string> grid_copy = grid;
    int score = 0;
    for (int i = 0; i < 100; i++) {
        score += simulate_octopuses(grid);
    }
    cout << "part1: " << score << endl;

    // part 2
    int count = 1;
    while (simulate_octopuses(grid_copy) != 100) {
        count++;
    }
    cout << "part2: " << count << endl;
    return 0;
}

int simulate_octopuses(vector<string>& grid) {
    queue<pair<int, int>> to_flash;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            grid[i][j]++;
            if (grid[i][j] > '9') {
                to_flash.push({i, j});
            }
        }
    }
    vector<vector<bool>> flashed(10, vector<bool>(10, false));
    while (!to_flash.empty()) {
        pair<int, int> p = to_flash.front();
        to_flash.pop();
        int i = p.first;
        int j = p.second;

        if (flashed[i][j] == false) {
            // increase all adjacent by 1
            bool can_left = i - 1 >= 0;
            bool can_right = i + 1 < 10;
            bool can_up = j - 1 >= 0;
            bool can_down = j + 1 < 10;
            if (can_left) {
                update_grid(grid, i-1, j, flashed);
                if (grid[i-1][j] > '9') {
                    to_flash.push({i-1, j});
                }
                if (can_up) {
                    update_grid(grid, i-1, j-1, flashed);
                    if (grid[i-1][j-1] > '9') {
                        to_flash.push({i-1, j-1});
                    }
                }
                if (can_down) {
                    update_grid(grid, i-1, j+1, flashed);
                    if (grid[i-1][j+1] > '9') {
                        to_flash.push({i-1, j+1});
                    }
                }
            }
            if (can_right) {
                update_grid(grid, i+1, j, flashed);
                if (grid[i+1][j] > '9') {
                    to_flash.push({i+1, j});
                }
                if (can_up) {
                    update_grid(grid, i+1, j-1, flashed);
                    if (grid[i+1][j-1] > '9') {
                        to_flash.push({i+1, j-1});
                    }
                }
                if (can_down) {
                    update_grid(grid, i+1, j+1, flashed);
                    if (grid[i+1][j+1] > '9') {
                        to_flash.push({i+1, j+1});
                    }
                }
            }
            if (can_up) {
                update_grid(grid, i, j-1, flashed);
                if (grid[i][j-1] > '9') {
                    to_flash.push({i, j-1});
                }
            }
            if (can_down) {
                update_grid(grid, i, j+1, flashed);
                if (grid[i][j+1] > '9') {
                    to_flash.push({i, j+1});
                }
            }
            // reset index
            grid[i][j] = '0';
            flashed[i][j] = true;
        }
    }
    int count = 0;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (flashed[i][j]) {
                count++;
            }
        }
    }
    return count;
}

void update_grid(vector<string>& grid, int i, int j, vector<vector<bool>>& flashed) {
    if (flashed[i][j] == false) {
        grid[i][j]++;
    }
}