#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

vector<pair<int, int>> get_low_points(const vector<string>& grid);
bool is_low_point(const vector<string>& grid, int i, int j, int n, int m);
int calculate_basin(const vector<string>& grid, vector<string>& visited, int i, int j, int n, int m);

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
    auto low_points = get_low_points(grid);
    int count = 0;
    for (pair p : low_points) {
        count += grid[p.first][p.second] - '0' + 1;
    }
    cout << "part1: " << count << endl;

    // part 2
    vector<int> basins;
    int n = grid.size();
    int m = grid[0].size();
    string zeros(m, '0');
    vector<string> visited(n, zeros);
    for (pair p : low_points) {
        basins.push_back(calculate_basin(grid, visited, p.first, p.second, n, m));
    }
    sort(basins.begin(), basins.end(), std::greater<>());
    long long result = 1;
    for (int i = 0; i < 3; i++) {
        result *= basins[i];
    }
    cout << "part2: " << result << endl;

    return 0;
}


vector<pair<int, int>> get_low_points(const vector<string>& grid) {
    vector<pair<int, int>> result;
    int n = grid.size();
    int m = grid[0].size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (is_low_point(grid, i, j, n, m)) {
                result.push_back({i, j});
            }
        }
    }
    return result;
}

bool is_low_point(const vector<string>& grid, int i, int j, int n, int m) {
    // since ordering of 0-9 in char is inorder, we can directly use char comparison
    if (i > 0) {
        if (grid[i][j] >= grid[i-1][j]) {return false;}
    }
    if (i < n - 1) {
        if (grid[i][j] >= grid[i+1][j]) {return false;}
    }
    if (j > 0) {
        if (grid[i][j] >= grid[i][j-1]) {return false;}
    }
    if (j < m - 1) {
        if (grid[i][j] >= grid[i][j+1]) {return false;}
    }

    return true;
}

int calculate_basin(const vector<string>& grid, vector<string>& visited, int i, int j, int n, int m) {
    visited[i][j] = '1';
    if (grid[i][j] == '9') {return 0;}
    int result = 1;
    if (i - 1 >= 0 && visited[i-1][j] == '0') {
        result += calculate_basin(grid, visited, i-1, j, n, m);
    }
    if (i + 1 < n && visited[i+1][j] == '0') {
        result += calculate_basin(grid, visited, i+1, j, n, m);
    }
    if (j - 1 >= 0 && visited[i][j-1] == '0') {
        result += calculate_basin(grid, visited, i, j-1, n, m);
    }
    if (j + 1 < m && visited[i][j+1] == '0') {
        result += calculate_basin(grid, visited, i, j+1, n, m);
    }
    return result;
}