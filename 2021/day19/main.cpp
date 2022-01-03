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
    vector<string> raw_lines = read_input(filename);
    for (auto l : raw_lines) {
        cout << l << endl;
    }
    // part 1

    // cout << "part1: " << simulate_sea_cucumbers(grid) << endl;

    // part 2
    // evidently using part1 strategy wont work, and I realised this problem is not really that 
    // "sparse"
    
    int result = 0;
    cout << "part2: " << result << endl;

    return 0;
}