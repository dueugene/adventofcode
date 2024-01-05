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

enum Direction { Up, Down, Left, Right };

struct PosHist {
    PosHist(Point2d<int> p_in, Direction d_in, int count_in, int score_in)
        : p(p_in), d(d_in), curr_dir_count(count_in), curr_score(score_in) {}
    Point2d<int> p;
    Direction d;
    int curr_dir_count;
    int curr_score;
};

struct Node {
    Node(Point2d<int> p_in, size_t avail_in, size_t score_in)
        : p(p_in), avail_connections(avail_in), curr_score(score_in) {}
    Point2d<int> p;
    size_t avail_connections;
    size_t curr_score;
};

class Compare {
   public:
    bool operator()(Node& a, Node& b) { return a.curr_score < b.curr_score; }
};

namespace std {
template <>
struct hash<Node> {
    auto operator()(const Node& point) const -> size_t {
        size_t lhs = hash<Point2d<int>>()(point.p);
        size_t rhs = point.avail_connections;
        lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
        return lhs;
    }
};
}  // namespace std

struct QuadrantPoint {
    QuadrantPoint(Point2d<int> p_in, Point2d<int> q_in) : p(p_in), q(q_in) {}
    Point2d<int> p;
    Point2d<int> q;
    bool operator==(const QuadrantPoint& other) const { return this->p == other.p && this->q == other.q; }
};
namespace std {
template <>
struct hash<QuadrantPoint> {
    auto operator()(const QuadrantPoint& point) const -> size_t {
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
        m_ = input[n_ - 1].size();
        step_count = 0;
        prev_score = 0;
        curr_score = 0;
        for (int i = 0; i < n_; ++i) {
            for (int j = 0; j < m_; ++j) {
                if (input[i][j] == 'S') {
                    input[i][j] = '.';
                    start = Point2d<int>(j, i);
                    avail.insert(start);
                    avail_qp.insert(QuadrantPoint(Point2d<int>(j, i), Point2d<int>(0, 0)));
                } else if (input[i][j] == '#') {
                    QuadrantPoint qp(Point2d<int>(j, i), Point2d<int>(0, 0));
                    rocks.insert(qp);
                }
            }
        }
        cout << "n: " << n_ << " m: " << m_ << endl;
    }
    int step() {
        ++step_count;
        unordered_set<Point2d<int>> new_set;
        for (auto p : avail) {
            for (auto next : get_neighbors(p)) {
                if (input[next.y_][next.x_] == '.') {
                    new_set.insert(next);
                }
            }
        }
        avail = new_set;
        return avail.size();
    }
    unsigned long long step_infinite() {
        ++step_count;
        unordered_set<QuadrantPoint> new_set;
        for (auto p : avail_qp) {
            for (auto& next : get_qp_neighbors(p)) {
                if (input[next.p.y_][next.p.x_] == '.' && prev_qp.find(next) == prev_qp.end()) {
                    new_set.insert(next);
                }
            }
        }
        if (false && step_count == 10) {
            cout << "step: " << step_count << " " << new_set.size() << endl;
            for (auto p : new_set) {
                cout << "next: " << p.p << " " << p.q << endl;
            }
        }

        unsigned long long temp = curr_score;
        curr_score = prev_score + new_set.size();
        prev_score = temp;
        if (step_count % 1000 == 0) {
            cout << "step: " << step_count << " " << prev_qp.size() << " " << avail_qp.size() << " " << new_set.size()
                 << endl;
            cout << curr_score << endl;
        }
        prev_qp = move(avail_qp);
        avail_qp = move(new_set);
        return curr_score;
    }
    unsigned long long step_infinite2() {
        ++step_count;
        unordered_set<QuadrantPoint> new_set;
        unordered_map<Point2d<int>, unordered_set<Point2d<int>>> new_set2;
        for (auto p : avail_qp) {
            for (auto& next : get_qp_neighbors(p)) {
                if (input[next.p.y_][next.p.x_] == '.') {
                    new_set2[next.q].insert(next.p);
                }
            }
        }

        unsigned long long temp = curr_score;
        curr_score = prev_score + new_set.size();

        return curr_score;
    }
    void reset() {
        avail.clear();
        avail.insert(start);
        step_count = 0;
        prev_score = 0;
        curr_score = 1;
    }
    int get_step_count() const { return step_count; }
    friend ostream& operator<<(ostream& os, const Garden& other);

   private:
    list<QuadrantPoint> get_qp_neighbors(const QuadrantPoint& p) {
        list<QuadrantPoint> result;
        result.push_back({Point2d<int>(p.p.x_ - 1, p.p.y_), p.q});
        result.push_back({Point2d<int>(p.p.x_ + 1, p.p.y_), p.q});
        result.push_back({Point2d<int>(p.p.x_, p.p.y_ - 1), p.q});
        result.push_back({Point2d<int>(p.p.x_, p.p.y_ + 1), p.q});
        for (auto& pq : result) {
            if (pq.p.x_ < 0) {
                // cout << "here: " << pq.p << " " << pq.q << endl;
                pq.p.x_ = m_ - 1;
                pq.q.x_ -= 1;
                // cout << "here: " << pq.p << " " << pq.q << endl;
            } else if (pq.p.x_ >= m_) {
                pq.p.x_ = 0;
                pq.q.x_ += 1;
            }
            if (pq.p.y_ < 0) {
                pq.p.y_ = n_ - 1;
                pq.q.y_ -= 1;
            } else if (pq.p.y_ >= n_) {
                pq.p.y_ = 0;
                pq.q.y_ += 1;
            }
        }
        return result;
    }
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
    unordered_set<QuadrantPoint> avail_qp;
    unordered_set<QuadrantPoint> prev_qp;
    unordered_set<QuadrantPoint> rocks;
    unsigned long long prev_score;
    unsigned long long curr_score;
};

ostream& operator<<(ostream& os, const Garden& other) {
    for (int i = 0; i < other.n_; ++i) {
        for (int j = 0; j < other.m_; ++j) {
            if (other.avail.find(Point2d<int>(j, i)) != other.avail.end()) {
                os << "O";
            } else if (other.rocks.find(QuadrantPoint(Point2d<int>(j, i), Point2d<int>(0, 0))) != other.rocks.end()) {
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
        part1 = g.step();
        cout << "step: " << g.get_step_count() << " " << part1 << endl;
        cout << g << endl;
    }
    g.reset();
    unsigned long long part2 = 0;
    for (int i = 0; i < 26501365; i++) {
        part2 = g.step_infinite();
        // cout << "step: " << g.get_step_count() << " " << part2 << endl;
    }

    // part 1
    cout << "part1: " << part1 << endl;
    // part 2
    cout << "part2: " << part2 << endl;
    return 0;
}

// ...........
// .....###.#.
// .###.##.O#.
// .O#O#8.O#..
// 8.O.#.#.O..
// .##O.O####.
// .##.O#O..#.
// .O.O.8.##..
// .##.#.####.
// .##8.##.##.
// ...........