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

// copy from https://www.techiedelight.com/use-std-pair-key-std-unordered_map-cpp/
struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2> &pair) const {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

class PointsGrid {
public:
    PointsGrid(vector<pair<int, int>> coords) {
        coords_.insert(coords.begin(), coords.end());
    }

    int reflect_line(string line) {
        char c = line[0];
        int b = stoi(line.substr(2));

        unordered_set<pair<int, int>, pair_hash> update;
        if (c == 'x') {
            for (auto p : coords_) {
                if (p.first > b) {
                    p.first = b - (p.first - b);
                }
                update.insert(p);
            }
        } else if (c == 'y') {
            for (auto p : coords_) {
                if (p.second > b) {
                    p.second = b - (p.second - b);
                }
                update.insert(p);
            }
        }
        coords_ = update;
        return coords_.size();
    }

    void print_coords() const {
        for (auto p : coords_) {
            cout << p.first << ", " << p.second << endl;
        }
    }

    void print_image() const {
        int x_min = INT_MAX;
        int x_max = INT_MIN;
        int y_min = INT_MAX;
        int y_max = INT_MIN;
        for (auto p : coords_) {
            x_min = min(p.first, x_min);
            x_max = max(p.first, x_max);
            y_min = min(p.second, y_min);
            y_max = max(p.second, y_max);
        }
        cout << x_min << ", " << y_min << " " << x_max << ", " << y_max << endl;
        for (int i = y_min; i <= y_max; i++) {
            for (int j = x_min; j <= x_max; j++) {
                if (coords_.find({j,i}) != coords_.end()) {
                    cout << '#';
                } else {
                    cout << '.';
                }
            }
            cout << endl;
        }
    }

private:
    unordered_set<pair<int, int>, pair_hash> coords_;
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
    ifstream input;
    input.open(filename);
    string line;
    vector<pair<int, int>> coords;
    while (getline(input, line)) {
        if (line.empty()) {break;}
        istringstream ss(line);
        int a, b;
        char c;
        ss >> a >> c >> b;
        coords.push_back({a, b});
    }
    string a, b, c;
    vector<string> commands;
    while (input >> a >> b >> c) {
        commands.push_back(c);
    }

    input.close();

    // part 1
    PointsGrid points_grid(coords);
    cout << "part1: " << points_grid.reflect_line(commands[0]) << endl;

    // part 2
    for (int i = 1; i < commands.size(); i++) {
        points_grid.reflect_line(commands[i]);
    }
    points_grid.print_image();
    return 0;
}
