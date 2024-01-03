#include <iostream>
#include <fstream>
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
#include "grids.hpp"

using namespace std;

template <typename T>
using Matrix = vector<vector<T>>;
using ulong = unsigned long long;




class PipeMap {
public:
    PipeMap(const Matrix<char>& input) : grid_(input) {
        n_ = grid_.size();
        m_ = grid_[0].size();
        find_s();
        Point2d left(-1, 0);
        Point2d right(1, 0);
        Point2d above(0, -1);
        Point2d below(0, 1);
        key_types_ = vector<unordered_set<Point2d>>(255);
        key_types_['|'].insert({above, below}); // is a vertical pipe connecting north and south.
        key_types_['-'].insert({left, right}); // is a horizontal pipe connecting east and west.
        key_types_['L'].insert({above, right}); // is a 90-degree bend connecting north and east.
        key_types_['J'].insert({left, above}); // is a 90-degree bend connecting north and west.
        key_types_['7'].insert({left, below}); // is a 90-degree bend connecting south and west.
        key_types_['F'].insert({below, right}); // is a 90-degree bend connecting south and east.
        key_types_['S'].insert({above, below, left, right});
    }

    int find_longest_loop() {
        queue<Point2d> q;
        unordered_set<Point2d> visited;
        list<Point2d> dir1;
        list<Point2d> dir2;
        q.push(s_ind_);
        while (!q.empty()) {
            int k = q.size();
            if (k > 2) {
                cout << "WARNING: ABOVE 2" << endl;
            }
            while (k--) {
                Point2d curr = q.front();
                q.pop();
                // cout << curr << endl;
                visited.insert(curr);
                if (dir1.size() == dir2.size()) {
                    dir1.push_back(curr);
                } else {
                    dir2.push_back(curr);
                }
                for (auto neighbor : get_neighbors(curr)) {
                    if (is_valid(neighbor) && visited.find(neighbor) == visited.end()) {
                        auto temp = get_neighbors(neighbor);
                        if (temp.find(curr) != temp.end()) {
                            // cout << " " << neighbor << endl;
                            q.push(neighbor);
                        }
                    }
                }
            }
        }
        clean_unused_pipes(visited);
        if (dir1.back() == dir2.back()) {
            dir2.pop_back();
        }
        circular_path_.insert(circular_path_.end(), dir1.begin(), dir1.end());
        circular_path_.insert(circular_path_.end(), dir2.rbegin(), dir2.rend());
        // for (auto& k : grid_) {
        //     for (auto& c : k) {
        //         cout << c;
        //     }
        //     cout << endl;
        // }
        // for (auto a : circular_path_) {
        //     cout << a << endl;
        // }
        return circular_path_.size()/2;
    }

    int find_enclosed_tiles() {
        // travel loop, and count islands to the "right" of the loop
        // we only need to consider tiles '|' and '-'
        // we need to consider the direction we're travelling 
        // '|' and up, to the right is (1, 0)
        // '|' and down, to the right is(-1, 0)
        // '-' and right, to the right is (0, 1)
        // '-' and left, to the right is (0, -1)
        // note: I lied, I need to check F, J, 7, and L as well
        Point2d prev = circular_path_.front();
        for (const auto& p : circular_path_) {
            Point2d dir = p - prev;
            Point2d test_point;
            switch (grid_[p.y_][p.x_]) {
            case '|':
                if (dir.y_ == 1) {
                    test_point = p + Point2d(-1, 0);
                } else {
                    test_point = p + Point2d(1, 0);
                }
                check_and_fill_island(test_point, 'I');
                break;
            case '-':
                if (dir.x_ == 1) {
                    test_point = p + Point2d(0, 1);
                } else {
                    test_point = p + Point2d(0, -1);
                }
                check_and_fill_island(test_point, 'I');
                break;
            case 'F':
                if (dir.y_ == -1) {

                } else {
                    test_point = p + Point2d(-1, 0);
                    check_and_fill_island(test_point, 'I');
                    test_point = p + Point2d(0, -1);
                    check_and_fill_island(test_point, 'I');

                }
                break;
            case '7':
                if (dir.x_ == 1) {

                } else {
                    test_point = p + Point2d(1, 0);
                    check_and_fill_island(test_point, 'I');
                    test_point = p + Point2d(0, -1);
                    check_and_fill_island(test_point, 'I');
                }
                break;
            case 'L':
                if (dir.x_ == -1) {

                } else {
                    test_point = p + Point2d(-1, 0);
                    check_and_fill_island(test_point, 'I');
                    test_point = p + Point2d(0, 1);
                    check_and_fill_island(test_point, 'I');
                }
                break;
            case 'J':
                if (dir.y_ == 1) {

                } else {
                    test_point = p + Point2d(1, 0);
                    check_and_fill_island(test_point, 'I');
                    test_point = p + Point2d(0, 1);
                    check_and_fill_island(test_point, 'I');
                }
                break;
            }
            prev = p;
        }
        // add up all the inner points
        int inner_count = 0;
        int outer_count = 0;
        for (int i = 0; i < n_; i++) {
            for (int j = 0; j < m_; j++) {
                switch (grid_[i][j]) {
                case '.':
                    outer_count++;
                    break;
                case 'I':
                    inner_count++;
                    break;
                }
            }
        }
        // my code ended up finding the outer tiles for the input data, so I just print both here
        cout << n_ << " x " << m_ << endl;
        cout << "inner: " << inner_count << endl;
        cout << "outer: " << outer_count << endl;
        cout << "loop: " << circular_path_.size() << endl;
        for (auto& k : grid_) {
            for (auto& c : k) {
                cout << c;
            }
            cout << endl;
        }
        return inner_count;
    }
private:
    void check_and_fill_island(const Point2d& p, char input) {
        if (is_valid(p) && grid_[p.y_][p.x_] == '.') {
            grid_[p.y_][p.x_] = 'S';
            for (auto next : get_neighbors(p)) {
                check_and_fill_island(next, input);
            }
        grid_[p.y_][p.x_] = input;
        } else {
            return;
        }
    }
    void clean_unused_pipes(const unordered_set<Point2d>& points) {
        for (int i = 0; i < n_; i++) {
            for (int j = 0; j < m_; j++) {
                if (points.find(Point2d(j, i)) == points.end()) {
                    grid_[i][j] = '.';
                }
            }
        }
    }
    void find_s() {
        for (int i = 0; i < n_; i++) {
            for (int j = 0; j < m_; j++) {
                if (grid_[i][j] == 'S') {
                    s_ind_ = Point2d(j, i);
                    return;
                }
            }
        }
    }
    unordered_set<Point2d> get_neighbors(const Point2d& p) const {
        unordered_set<Point2d> result;
        for (auto k : key_types_[grid_[p.y_][p.x_]]) {
            result.insert(p + k);
        }
        return result;
    }
    bool is_valid(const Point2d& p) const {
        return p >= Point2d(0, 0) && p < Point2d(m_, n_);
    }
    Matrix<char> grid_;
    int n_;
    int m_;
    Point2d s_ind_;
    vector<unordered_set<Point2d>> key_types_;
    list<Point2d> circular_path_;
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
    string a;
    Matrix<char> input_data = input_reader::read_as_matrix(filename);
    PipeMap pipe_map(input_data);

    ulong part1 = pipe_map.find_longest_loop();
    ulong part2 = pipe_map.find_enclosed_tiles();

    // part 1
    cout << "part1: " << part1 << endl;
    // part 2
    cout << "part2: " << part2 << endl;
    return 0;
}