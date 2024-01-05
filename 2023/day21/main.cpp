#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <set>
#include <sstream>
#include <list>
#include <algorithm>
#include <cmath>
#include <numeric>

#include "grids.hpp"
#include "input_reader.hpp"

using namespace std;

template <typename T>
using Matrix = vector<vector<T>>;

enum Direction {Up, Down, Left, Right};

struct QuadrantPoint {
    QuadrantPoint(Point2d<int> p_in, Point2d<int> q_in) : p(p_in), q(q_in) {}
    Point2d<int> p;
    Point2d<int> q;
    bool operator==(const QuadrantPoint& other) const {
        return this->p == other.p && this->q == other.q;
    }
};
namespace std {
template <>
struct hash<QuadrantPoint> {
  auto operator()(const QuadrantPoint &point) const -> size_t {
    size_t lhs = hash<Point2d<int>>()(point.p);
    size_t rhs = hash<Point2d<int>>()(point.q);
    lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
    return lhs;
  }
};
}  // namespace std


class Garden {
public:
    Garden(Matrix<char> input_in) : input(input_in) {
        n_ = input.size();
        m_ = input[n_-1].size();
        step_count = 0;
        for (int i = 0; i < n_; ++i) {
            for (int j = 0; j < m_; ++j) {
                if (input[i][j] == 'S') {
                    input[i][j] = '.';
                    start = Point2d<int>(j, i);
                    avail.insert(start);
                } else if (input[i][j] == '#') {
                    rocks.insert(Point2d<int>(j, i));
                }
            }
        }
        cout << "n: " << n_ << " m: " << m_ << endl;
    }

    Garden(const Garden& other) {
        input = other.input;
        n_ = other.n_;
        m_ = other.m_;
        step_count = other.step_count;
        start = other.start;
        avail = other.avail;
        rocks = other.rocks;
    }

    list<Point2d<int>> step() {
        ++step_count;
        list<Point2d<int>> result;
        unordered_set<Point2d<int>> new_set;
        for (auto p : avail) {
          for (auto next : get_neighbors(p)) {
            if (input[next.y_][next.x_] == '.') {
                new_set.insert(next);
            }
          }
        }
        avail = new_set;
        return result;
    }

    void set_start(Point2d<int> start_in) {
        start = start_in;
    }
    void reset() {
        avail.clear();
        avail.insert(start);
        step_count = 0;
    }

    int get_step_count() const {
        return step_count;
    }

    int get_score() const {
        return avail.size();
    }

    unordered_set<Point2d<int>> get_avail() const {
        return avail;
    }

    friend ostream& operator<<(ostream& os, const Garden& other);
    friend struct hash<Garden>;
    bool operator==(const Garden& other) const {
        return this->avail == other.avail;
    }
private:
  list<Point2d<int>> get_neighbors(Point2d<int> p) {
    list<Point2d<int>> result;
    if (p.x_ > 0) {
      result.push_back(Point2d<int>(p.x_ - 1, p.y_));
    }
    if (p.x_ < m_ - 1) {
      result.push_back(Point2d<int>(p.x_ + 1, p.y_));
    }
    if (p.y_ > 0) {
      result.push_back(Point2d<int>(p.x_, p.y_ - 1));
    }
    if (p.y_ < n_ - 1) {
      result.push_back(Point2d<int>(p.x_, p.y_ + 1));
    }
    return result;
  }
  int step_count;
  Matrix<char> input;
  int n_;
  int m_;
  Point2d<int> start;
  unordered_set<Point2d<int>> avail;
  unordered_set<Point2d<int>> rocks;
};

namespace std {
template <>
struct hash<Garden> {
  auto operator()(const Garden &g) const -> size_t {
    size_t lhs = 0;
    for (auto& p : g.avail) {
        size_t rhs = hash<Point2d<int>>()(p);
        lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
    }
    return lhs;
  }
};
}  // namespace std

ostream& operator<<(ostream& os, const Garden& other) {
    for (int i = 0; i < other.n_; ++i) {
        for (int j = 0; j < other.m_; ++j) {
            if (other.avail.find(Point2d<int>(j, i)) != other.avail.end()) {
                os << "O";
            } else if (other.rocks.find(Point2d<int>(j, i)) != other.rocks.end()) {
                os << "#";
            } else {
                os << other.input[i][j];
            }
        }
        os << endl;
    }
    return os;
}



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
    Matrix<char> input_matrix = input_reader::read_as_matrix(filename);

    unsigned long long part1 = 0;
    Garden g(input_matrix);
    for (int i = 0; i < 64; i++) {
        g.step();
        // cout << "step: " << g.get_step_count() << " " << part1 << endl;
        // cout << g << endl;
    }
    part1 = g.get_score();
    g.reset();

    unsigned long long part2 = 0;
    // DID NOT WORK!!
    // observations
    // 1. eventually when the board is saturated, the occupied grid points will oscilate between 2 values
    // 2. we won't need to "simulate" at this point and just toggle back and forth
    // 3. as we enter a "new" grid area, its like staring a new simulation with a new starting point
    // eg. looking to the tile to the right
    //      .0.      0.0  ...
    //      0.0   -> .0.  0..
    //      .0.      0.0  ...
    // 4. presumably some tiles will be the same as before or have same start, so we can cache the results

    // cache to that maps starting point to score history of garden
    // I am
    // unordered_map<pair<Point2d<int>,int>, vector<int>> garden_score_history;
    unordered_map<Garden, vector<int>> garden_score_history;
    queue<Garden> gq;
    gq.push(g);
    for (int i = 0; i < 3; i++) {//26501365; i++) {
        list<Point2d<int>> outer_points = g.step();
        for (auto& p : outer_points) {
            cout << p << endl;
            Garden g2(g);
            g2.set_start(p);
            g2.reset();
            gq.push(g2);
        }
        cout << "step: " << g.get_step_count() << " " << part2 << endl;
    }

    // part 1
    cout << "part1: " << part1 << endl;
    // part 2
    cout << "part2: " << part2 << endl;
    return 0;
}