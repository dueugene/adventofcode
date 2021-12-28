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

void djikstra(vector<string>& grid, vector<int>& dist, vector<int>& prev);
int find_lowest_dist(const unordered_set<int>& nodes, const vector<int>& dist);
vector<string> retile_grid(const vector<string>& grid);

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
    ifstream input;
    input.open(filename);
    string line;
    vector<string> grid;
    while (getline(input, line)) {
        grid.push_back(line);
    }
    for (auto p : grid) {
        cout << p << endl;
    }
    input.close();

    // part 1
    // first attempt: dfs with visited nodes list
    // second attempt: add memo list
    // third attempt: dijkstras (from browswing reddit forum)
    vector<int> dist;
    vector<int> prev;
    djikstra(grid, dist, prev);
    int dest = grid.size() * grid[0].size() - 1;
    cout << "part1: " << dist[dest] << endl;


    // part 2
    grid = retile_grid(grid);
    djikstra(grid, dist, prev);
    dest = grid.size() * grid[0].size() - 1;
    cout << "part2: " << dist[dest] << endl;
    
    return 0;
}

void djikstra(vector<string>& grid, vector<int>& dist, vector<int>& prev) {
    int n = grid.size();
    int m = grid[0].size();

    unordered_set<int> nodes;
    dist.resize(n*m);
    prev.resize(n*m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int key = i * m + j;
            dist[key] = INT_MAX;
            nodes.insert(key);
        }
    }
    dist[0] = 0;
    while (nodes.empty() == false) {
        int key = find_lowest_dist(nodes, dist);
        nodes.erase(key);
        int i = key / m;
        int j = key % m;
        if (i == n - 1 && j == m - 1) {break;}
        // for each neighber of current (i, j)
        if (i + 1 < n) { 
            int next = (i+1) * m + j;
            if (nodes.find(next) != nodes.end()) {
                int candidate = dist[key] + grid[i+1][j] - '0';
                if (candidate < dist[next]) {
                    dist[next] = candidate;
                    prev[next] = key;
                }
            }
        }
        if (j + 1 < m) { 
            int next = (i) * m + j+1;
            if (nodes.find(next) != nodes.end()) {
                int candidate = dist[key] + grid[i][j+1] - '0';
                if (candidate < dist[next]) {
                    dist[next] = candidate;
                    prev[next] = key;
                }
            }
        }
        if (i - 1 >= 0) { 
            int next = (i-1) * m + j;
            if (nodes.find(next) != nodes.end()) {
                int candidate = dist[key] + grid[i-1][j] - '0';
                if (candidate < dist[next]) {
                    dist[next] = candidate;
                    prev[next] = key;
                }
            }
        }
        if (j - 1 >= 0) { 
            int next = (i) * m + j-1;
            if (nodes.find(next) != nodes.end()) {
                int candidate = dist[key] + grid[i][j-1] - '0';
                if (candidate < dist[next]) {
                    dist[next] = candidate;
                    prev[next] = key;
                }
            }
        }
        
    }

}

int find_lowest_dist(const unordered_set<int>& nodes, const vector<int>& dist) {
    int ind = *nodes.begin();
    int lowest = dist[ind];
    for (int key : nodes) {
        if (dist[key] < lowest) {
            lowest = dist[key];
            ind = key;
        }
    }
    return ind;
}

vector<string> retile_grid(const vector<string>& grid) {
    int n = grid.size();
    int m = grid[0].size();
    vector<string> result(n*5, string(m*5, '0'));

    for (int k = 0; k < 5; k++) {
        for (int l = 0; l < 5; l++) {
            int ibeg = k * n;
            int jbeg = l * m;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    int c = grid[i][j] + k + l - '0';
                    if (c > 9) {
                        c %= 9;
                    }
                    result[ibeg + i][jbeg + j] = c + '0';
                }
            }
        }
    }
    return result;
}