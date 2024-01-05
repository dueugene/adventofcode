#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "grids.hpp"
#include "input_reader.hpp"

using namespace std;

template <typename T>
using Matrix = vector<vector<T>>;

void split_string_data(const string& s, char& c, int& num, string& s2) {
    c = s[0];
    int i = 2;
    num = 0;
    while (s[i] >= '0' && s[i] <= '9') {
        num *= 10;
        num += s[i] - '0';
        i++;
    }
    s2 = s.substr(i + 2, 7);
}

bool valid_point(Matrix<char>& map, Point2d<int> start) {
    int n = map.size();
    int m = map[0].size();
    return start.x_ >= 0 && start.x_ < m && start.y_ >= 0 && start.y_ < n;
}
list<Point2d<int>> get_neighbors(Matrix<char>& map, Point2d<int> start) {
    list<Point2d<int>> res;
    list<Point2d<int>> dirs = {
        {0, -1},
        {0, 1},
        {-1, 0},
        {1, 0},
    };
    for (auto dir : dirs) {
        auto cand = start + dir;
        if (valid_point(map, cand) && map[cand.y_][cand.x_] == '.') {
            res.push_back(cand);
        }
    }
    return res;
}

void flood_fill(Matrix<char>& map, Point2d<int> start) {
    queue<Point2d<int>> q;
    q.push(start);
    map[start.y_][start.x_] = '#';
    while (!q.empty()) {
        auto curr = q.front();
        q.pop();
        for (auto neigh : get_neighbors(map, curr)) {
            map[neigh.y_][neigh.x_] = '#';
            q.push(neigh);
        }
    }
}

int from_hex(const string& s) {
    int res = 0;
    for (int i = 0; i < s.size(); i++) {
        res *= 16;
        if (s[i] >= '0' && s[i] <= '9') {
            res += s[i] - '0';
        } else {
            res += s[i] - 'a' + 10;
        }
    }
    return res;
}

unsigned long long computePolygonArea(const std::vector<Point2d<int>>& vertices) {
    int64_t area = 0;
    int64_t v = 1;
    int j = vertices.size() - 1;

    for (int i = 0; i < vertices.size(); i++) {
        int64_t a = int64_t(vertices[j].x_) * int64_t(vertices[i].y_);
        int64_t b = int64_t(vertices[i].x_) * int64_t(vertices[j].y_);
        cout << vertices[j] << " " << vertices[i] << endl;
        cout << a << " " << b << endl;
        area += (a - b);
        j = i;
    }
    area = area / 2;
    return std::abs(static_cast<long long>(area));
}

unsigned long long computePolygonPerimeter(const std::vector<Point2d<int>>& vertices) {
    unsigned long long perimeter = 0;
    int j = vertices.size() - 1;

    for (int i = 0; i < vertices.size(); i++) {
        int dx = abs(vertices[j].x_ - vertices[i].x_);
        int dy = abs(vertices[j].y_ - vertices[i].y_);
        perimeter += dx + dy;
        j = i;
    }

    return perimeter;
}

// unsigned long long computePolygonArea(const std::vector<Point2d<int>>& vertices) {
//     long long area = 0.0;
//     int j = vertices.size() - 1;

//     for (int i = 0; i < vertices.size(); i++) {
//         area += (vertices[j].x_ + vertices[i].x_) * (vertices[j].y_ - vertices[i].y_);
//         j = i;
//     }

//     return std::abs(area / 2);
// }

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
    vector<string> input_data = input_reader::read_as_strings(filename);

    ulong part1 = 0;

    list<Point2d<int>> perimeter;
    Point2d<int> curr(0, 0);
    perimeter.push_back(curr);
    vector<string> colors;
    vector<int> dists;
    vector<char> dirs;
    for (auto s : input_data) {
        char dir;
        int dist;
        string color;
        split_string_data(s, dir, dist, color);
        dirs.push_back(dir);
        dists.push_back(dist);
        colors.push_back(color);
        Point2d<int> new_dir;
        switch (dir) {
            case 'U':
                new_dir = {0, -1};
                break;
            case 'D':
                new_dir = {0, 1};
                break;
            case 'L':
                new_dir = {-1, 0};
                break;
            case 'R':
                new_dir = {1, 0};
                break;
        }
        while (dist--) {
            curr = curr + new_dir;
            perimeter.push_back(curr);
        }
    }
    // print
    int x_min = 0, x_max = 0;
    int y_min = 0, y_max = 0;
    for (auto p : perimeter) {
        x_min = min(p.x_, x_min);
        x_max = max(p.x_, x_max);
        y_min = min(p.y_, y_min);
        y_max = max(p.y_, y_max);
    }
    int n = y_max - y_min + 1;
    int m = x_max - x_min + 1;
    Matrix<char> map(n, vector<char>(m, '.'));
    for (auto p : perimeter) {
        map[p.y_ - y_min][p.x_ - x_min] = '#';
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << map[i][j];
        }
        cout << endl;
    }
    // first attempt for part1, flood fill the enclosed area, but does not work for part2
    // Point2d<int> start((x_max - x_min)/2 + x_min, (y_max - y_min)/2 + y_min);
    // Point2d<int> start(252, 16);

    // flood_fill(map, start);
    // for (int i = 0; i < n; i++) {
    //     for (int j = 0; j < m; j++) {
    //         if (map[i][j] == '#') {
    //             part1++;
    //         }
    //         cout << map[i][j];
    //     }
    //     cout << endl;
    // }
    vector<Point2d<int>> vertices;
    Point2d<int> start(0, 0);
    ulong part2 = 0;
    for (auto c : colors) {
        cout << c << endl;
        char dir;
        int lenght = from_hex(c.substr(1, 5));
        switch (c[c.size() - 1]) {
            case '0':
                dir = 'R';
                start = start + Point2d<int>(lenght, 0);
                vertices.push_back(start);
                break;
            case '1':
                dir = 'D';
                start = start + Point2d<int>(0, lenght);
                vertices.push_back(start);
                break;
            case '2':
                dir = 'L';
                start = start + Point2d<int>(-lenght, 0);
                vertices.push_back(start);
                break;
            case '3':
                dir = 'U';
                start = start + Point2d<int>(0, -lenght);
                vertices.push_back(start);
                break;
        }
        cout << dir << " " << lenght << endl;
    }

    for (auto v : vertices) {
        cout << v << endl;
    }
    // part2, found the formula for area of a polygon
    // but it did not quite work, had to inlcude half the perimeter + 1
    cout << computePolygonArea(vertices) << endl;
    cout << computePolygonPerimeter(vertices) << endl;
    cout << computePolygonArea(vertices) + computePolygonPerimeter(vertices) / 2 + 1 << endl;

    // part 1
    cout << "part1: " << part1 << endl;
    // part 2
    cout << "part2: " << part2 << endl;
    return 0;
}
