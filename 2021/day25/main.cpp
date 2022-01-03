#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>
#include <queue>
#include <unordered_set>
#include <list>
#include <memory>

#include "../utils/file_read.cpp"

using namespace std;

int simulate_sea_cucumbers(vector<string> grid);

int main(int argc, char** argv) {
    // this will allow different input files to be passed
    string filename;
    if (argc > 1) {
        filename = argv[1];
    } else {
        // default input
        filename = "sample.txt";
    }

    // read the input file
    vector<string> grid = read_input(filename);
    
    // part 1

    cout << "part1: " << simulate_sea_cucumbers(grid) << endl;

    // part 2
    // evidently using part1 strategy wont work, and I realised this problem is not really that 
    // "sparse"
    
    int result = 0;
    cout << "part2: " << result << endl;

    return 0;
}

int simulate_sea_cucumbers(vector<string> grid) {
    int n = grid.size();
    int m = grid[0].size();
    int count = 0;
    bool moving = true;
    while (moving) {
        for (auto line : grid) {
            cout << line << endl;
        }
        cout << endl;
        moving = false;
        vector<string> next = grid; // (n, string(m, '.'));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (grid[i][j] == '>') {
                    int j_next = (j + 1) % m;
                    if (grid[i][j_next] == '.') {
                        swap(next[i][j], next[i][j_next]);
                        moving = true;
                    } else {
                        // next[i][j] = '>';
                    }
                } else {
                    // next[i][j] = grid[i][j];
                }
            }
        }
        grid = next;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (next[i][j] == 'v') {
                    int i_next = (i + 1) % n;
                    if (next[i_next][j] == '.') {
                        swap(grid[i][j], grid[i_next][j]);
                        moving = true;
                    } else {
                        // next[i][j] = 'v';
                    }
                }
            }
        }
        grid;
        count++;
    }
    return count;
}
