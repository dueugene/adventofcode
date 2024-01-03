#include <iostream>
#include <map>
#include <queue>
#include <unordered_set>
#include <list>

#include "grids.hpp"
#include "input_reader.hpp"

using namespace std;

template <typename T>
using Matrix = vector<vector<T>>;

enum Direction {Up, Down, Left, Right};
struct Pos2d {
    Pos2d(Point2d<int> p_in, Direction dir_in) : p(p_in), dir(dir_in) {}
    Point2d<int> p;
    Direction dir;
    bool operator==(const Pos2d& other) const {
        return p == other.p && dir == other.dir;
    }
};

namespace std {
template <>
struct hash<Pos2d> {
  auto operator()(const Pos2d &p) const -> size_t {
    auto h = hash<Point2d<int>>{}(p.p);
    return p.dir ^ (h << 1);
  }
};
}  // namespace std

class LaserBoard {
public:
    LaserBoard(const Matrix<char>& input) : board_(input), board_begin_(input) {
        n_ = board_.size();
        m_ = board_[0].size();
        splitter_[Up] = Right;
        splitter_[Down] = Left;
        splitter_[Left] = Down;
        splitter_[Right] = Up;
        splitter2_[Up] = Left;
        splitter2_[Down] = Right;
        splitter2_[Left] = Up;
        splitter2_[Right] = Down;

    }

    int find_energized_tiles(Pos2d start) {
        int result = 0;
        queue<Pos2d> q;
        unordered_set<Pos2d> seen;
        unordered_set<Point2d<int>> energized;
        q.push(start);
        while (!q.empty()) {
            Pos2d curr = q.front();
            q.pop();
            seen.insert(curr);
            energized.insert(curr.p);
            list<Pos2d> next;
            switch (board_[curr.p.y_][curr.p.x_]) {
            case '.':
                next = get_next(curr.p, curr.dir);
                break;
            case '/':
                next = get_next(curr.p, splitter_[curr.dir]);
                break;
            case '\\':
                next = get_next(curr.p, splitter2_[curr.dir]);
                break;
            case '|':
                if (curr.dir == Left || curr.dir == Right) {
                    next = get_next(curr.p, Up);
                    next.splice(next.end(), get_next(curr.p, Down));
                } else {
                    next = get_next(curr.p, curr.dir);
                }
                break;
            case '-':
                if (curr.dir == Up || curr.dir == Down) {
                    next = get_next(curr.p, Right);
                    next.splice(next.end(), get_next(curr.p, Left));
                } else {
                    next = get_next(curr.p, curr.dir);
                }
                break;
            
            default:
                break;
            }
            for (auto n : next) {
                if (seen.find(n) == seen.end()) {
                    q.push(n);
                }
            }
        }
        // for (int i = 0; i < n_; i++) {
        //     for (int j = 0; j < m_; j++) {
        //         Point2d<int> p(j, i);
        //         if (energized.find(p) != energized.end()) {
        //             cout << '#';
        //         } else {
        //             cout << board_[i][j];
        //         }
        //     }
        //     cout << endl;
        // }
        return energized.size();
    }
    list<Pos2d> get_next(Point2d<int> beg, Direction dir) {
        list<Pos2d> result;
        Point2d<int> next;
        switch (dir) {
        case Up:
            next = beg + Point2d<int>(0,-1);
            break;
        case Down:
            next = beg + Point2d<int>(0, 1);
            break;
        case Left:
            next = beg + Point2d<int>(-1, 0);
            break;
        case Right:
            next = beg + Point2d<int>(1, 0);
            break;
        }
        if (valid_pos(next)) {
            result.push_back(Pos2d(next, dir));
        }
        return result;
    }
    bool valid_pos(const Point2d<int>& p) const {
        return p >= Point2d<int>(0, 0) && p < Point2d<int>(m_, n_);
    }
    friend ostream& operator<<(ostream& os, const LaserBoard& other);
private:
    Matrix<char> board_;
    Matrix<char> board_begin_;
    map<Direction, Direction> splitter_;
    map<Direction, Direction> splitter2_;
    int n_;
    int m_;
};

ostream& operator<<(ostream& os, const LaserBoard& other) {
    for (int i = 0; i < other.n_; i++) {
        for (int j = 0; j < other.m_; j++) {
            os << other.board_[i][j];
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
    Matrix<char> input_data = input_reader::read_as_matrix(filename);
    LaserBoard lb(input_data);
    
    Pos2d start(Point2d(0,0), Right);
    ulong part1 = lb.find_energized_tiles(start);
    // part 1
    cout << "part1: " << part1 << endl;

    int part2 =  0;
    int n = input_data.size();
    int m = input_data[0].size();
    for (int i = 0; i < n; i++) {
        Pos2d start(Point2d(0, i), Right);
        int score = lb.find_energized_tiles(start);
        part2 = score > part2 ? score : part2;
                cout << score << endl;

        Pos2d star2t(Point2d(m-1, i), Left);
        score = lb.find_energized_tiles(star2t);
        part2 = score > part2 ? score : part2;
                cout << score << endl;

    }
    for (int j = 0; j < m; j++) {
        Pos2d start(Point2d(j,0), Down);
        int score = lb.find_energized_tiles(start);
        part2 = score > part2 ? score : part2;
        cout << score << endl;
        Pos2d star2t(Point2d(j,n-1), Up);
        score = lb.find_energized_tiles(star2t);
        part2 = score > part2 ? score : part2;
        cout << score << endl;
    }
    lb.find_energized_tiles({Point2d(3,0), Down});
    // part 2
    cout << "part2: " << part2 << endl;
    return 0;
}