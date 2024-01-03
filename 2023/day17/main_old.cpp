#include <iostream>
#include <list>
#include <set>
#include <unordered_map>
#include <queue>

#include "grids.hpp"
#include "input_reader.hpp"

using namespace std;

template <typename T>
using Matrix = vector<vector<T>>;
using u64int_t = unsigned long long;

enum Direction {Up, Down, Left, Right};

struct PosHist {
    PosHist(Point2d<int> p_in, Direction d_in, int count_in, int score_in) : p(p_in), d(d_in), curr_dir_count(count_in), curr_score(score_in) {}
	Point2d<int> p;
    Direction d;
    int curr_dir_count;
    int curr_score;
};

struct Node {
    Node(Point2d<int>p_in, size_t avail_in, size_t score_in) : p(p_in), avail_connections(avail_in), curr_score(score_in) {}
    Point2d<int> p;
    size_t avail_connections;
    size_t curr_score;
};

class Compare
{
public:
    bool operator() (Node& a, Node& b) {
        return a.curr_score < b.curr_score;
    }
};


namespace std {
template <>
struct hash<Node> {
  auto operator()(const Node &point) const -> size_t {
    size_t lhs = hash<Point2d<int>>()(point.p);
    size_t rhs = point.avail_connections;
    lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
    return lhs;
  }
};
}  // namespace std

ostream& operator<<(ostream& os, const PosHist& other) {
    os << other.p << ", " << other.d << ", " << other.curr_dir_count << ", " << other.curr_score;
    return os;
}

class LavaBoard {
public:
	LavaBoard(Matrix<char> input) {
		n_ = input.size();
		m_ = input[0].size();
        board_ = Matrix<int>(n_, vector<int>(m_,0));
		for (int i = 0; i < n_; i++) {
			for (int j = 0; j < m_; j++) {
				board_[i][j] = input[i][j] - '0';
			}
		}

	}

    int part1() {
        PosHist beg({0,0}, Right, 0, 0);
        Point2d<int> end(m_-1, n_-1);
        queue<PosHist> q;
        q.push(beg);
        int best_score = 1e3;
        while (!q.empty()) {
            auto curr = q.front();
            q.pop();
            cout << curr << endl;
            if (curr.p == end) {
                best_score = min(best_score, curr.curr_score);
            } else {
                for (auto next : get_neighbors(curr)) {
                    if (valid_point(next) && estimate_score(next) < best_score) {
                        next.curr_score = curr.curr_score + board_[next.p.y_][next.p.x_];
                        q.push(next);
                    }
                }
            }
        }
        return best_score;
    }

    void dijkstra() const {
        // setup all "nodes" in the graph
        // grid_[i][j] will represent up to 8 different nodes, depending on the direction it enters
        // example grid_[i][j] coming in right: 1. can leave up/down/right, 2. can leave up/down
        // we distinguish these two cases, as
        // priority_queue<Node, std::vector<Node>, Compare> pq;
        Node beg(Point2d<int>{0,0}, 8, 0);
        Node end(Point2d<int>{m_-1,n_-1}, 0, 1e8);
        auto cmp = [](const Node& a, const Node& b) {
            return a.curr_score < b.curr_score;
        };
        set<Node, decltype(cmp)> nodes(cmp);
        nodes.insert(beg);
        nodes.insert(end);
        while (!nodes.empty()) {
            auto it = nodes.begin();
            cout << it->p << " " << it->curr_score << endl;
            nodes.erase(it);
            // for each neighbor
        }

    }
    // size_t

private:
	Matrix<int> board_;
	int n_;
	int m_;
    list<PosHist> get_neighbors(PosHist curr) {
        list<PosHist> result;
        switch (curr.d) {
        case Up:
            result.push_back(move(curr, Up));
            result.push_back(move(curr, Left));
            result.push_back(move(curr, Right));
            break;
        case Down:
            result.push_back(move(curr, Down));
            result.push_back(move(curr, Left));
            result.push_back(move(curr, Right));
            break;
        case Left:
            result.push_back(move(curr, Up));
            result.push_back(move(curr, Left));
            result.push_back(move(curr, Down));
            break;
        case Right:
            result.push_back(move(curr, Up));
            result.push_back(move(curr, Right));
            result.push_back(move(curr, Down));
            break;
        }
        return result;
    }
    PosHist move(PosHist curr, Direction dir) {
        PosHist res(curr.p, dir, 1, curr.curr_score);
        if (dir == curr.d) {
            res.curr_dir_count = curr.curr_dir_count+1;
        }
        switch (dir) {
        case Up:
            res.p = res.p + Point2d<int>(0, -1);
            break;
        case Down:
            res.p = res.p + Point2d<int>(0, 1);
            break;
        case Left:
            res.p = res.p + Point2d<int>(-1, 0);
            break;
        case Right:
            res.p = res.p + Point2d<int>(1, 0);
            break;
        }
        return res;
    }
    bool valid_point(PosHist p) {
        if (p.curr_dir_count > 3) {
            return false;
        }
        return p.p >= Point2d<int>(0, 0) && p.p < Point2d<int>(m_, n_);
    }
    int estimate_score(PosHist& p) {
        int score = p.curr_score;
        score += n_ - 1 -p.p.y_;
        score += m_ - 1 - p.p.x_;
        score += board_[p.p.y_][p.p.x_];
        return score;
    }
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
    Matrix<char> input_data = input_reader::read_as_matrix(filename);

    LavaBoard lava_board(input_data);
    lava_board.dijkstra();
    u64int_t part1 = 0; //lava_board.part1();

    unordered_map<string, pair<int, int>> spin_cycle_map;
    // unordered_set<string> seen_boards;
    // list<string> order;
    unordered_map<int, int> imap;
    u64int_t part2 =  0;

    // part 1
    cout << "part1: " << part1 << endl;
    // part 2
    cout << "part2: " << part2 << endl;
    return 0;
}