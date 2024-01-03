#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <sstream>
#include <list>

#include "grids.hpp"
#include "input_reader.hpp"

using namespace std;

unordered_set<Point2d> get_neighbors(const Point2d& p, const Point2d& ul, const Point2d& br) {
    unordered_set<Point2d> result;
    Point2d cand(p);
    int& a = cand.x_;
    int& b = cand.y_;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            a = p.x_ + i;
            b = p.y_ + j;
            if (cand >= ul && cand <= br && cand != p) {
                result.insert(cand);
            }
        }
    }

    return result;
}

class Engine {
public:
    Engine(const vector<string>& grid_input) : grid_(grid_input) {
        ul_ = Point2d(0, 0);
        br_ = Point2d(grid_.size()-1, grid_[0].size()-1);
        for (int i = 0; i < grid_.size(); i++) {
            for (int j = 0; j < grid_[0].size(); j++) {
                char c = grid_[i][j];
                if (c == '.') {

                } else if (isdigit(c)) {
                    int k = j;
                    string num_builder;
                    while (j < grid_[0].size() && isdigit(c)) {
                        num_builder.push_back(c);
                        j++;
                        c = grid_[i][j];
                    }
                    j--;
                    part_numbers_.insert({Point2d(i, k), stoi(num_builder)});
                    // cout << num_builder << endl;
                } else {
                    // cout << i << " " << j << " " << c << endl;
                    symbols_.insert({Point2d(i, j), c});
                    if (c == '*') {
                        gears_.insert(Point2d(i, j));

                    }
                }
            }
        }
    }

    unordered_set<Point2d> get_adjacent_points(const Point2d& p, int n) {
        unordered_set<Point2d> result;
        int num = n;
        Point2d point = p;
        while (num) {
            result.merge(get_neighbors(point, ul_, br_));
            num /= 10;
            point.y_++;
        }
        return result;
    }

    int get_adjacent_part_numbers() {
        int result = 0;
        for (auto p : part_numbers_) {
            auto points = get_adjacent_points(p.first, p.second);
            bool connection = false;
            for (auto k : points) {
                if (symbols_.find(k) != symbols_.end()) {
                    connection = true;
                    if (gears_.find(k) != gears_.end()) {
                        gear_to_part_[k].push_back(p.second);
                    }
                }
            }
            if (connection) {
                result += p.second;
            }
        }
        return result;
    }

    int get_gear_ratios() {
        int result = 0;
        for (auto p : gear_to_part_) {
            if (p.second.size() == 2) {
                result += p.second[0] * p.second[1];
            }
        }
        return result;
    }
private:
    Point2d ul_;
    Point2d br_;
    vector<string> grid_;
    unordered_map<Point2d, int> part_numbers_;
    unordered_map<Point2d, char> symbols_;
    unordered_set<Point2d> gears_;
    unordered_map<Point2d, vector<int>> gear_to_part_;
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
    vector<string> grid = input_reader::read_as_strings(filename);
    
    Engine engine(grid);
    int part1 = 0;
    int part2 = 0;

    part1 = engine.get_adjacent_part_numbers();
    part2 = engine.get_gear_ratios();
    // part 1
    cout << "part1: " << part1 << endl;
    // part 2
    cout << "part2: " << part2 << endl;
    return 0;
}