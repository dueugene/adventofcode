#include <iostream>
#include <list>
#include <unordered_map>
#include <queue>

#include "grids.hpp"
#include "input_reader.hpp"

using namespace std;

template <typename T>
using Matrix = vector<vector<T>>;
using u64int_t = unsigned long long;

enum Direction {Up, Down, Left, Right};


struct Node {
    Node(Point2d<int>p_in, Direction d_in, size_t curr_score_in, size_t moves_in) : p(p_in), d(d_in), curr_score(curr_score_in), moves(moves_in) {

    }
    Point2d<int> p;
    Direction d;
    size_t curr_score;
    size_t moves;
    bool operator==(const Node& other) const {
        return this->p == other.p && this->d == other.d && this->moves == other.moves;
    }
};

namespace std {
template <>
struct hash<Node> {
  auto operator()(const Node &point) const -> size_t {
    size_t lhs = hash<Point2d<int>>()(point.p);
    size_t rhs = point.d;
    size_t moves = point.moves;
    lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
    lhs ^= moves + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
    return lhs;
  }
};
}  // namespace std

class Compare
{
public:
    bool operator() (Node& a, Node& b) {
        return a.curr_score > b.curr_score;
    }
};



class LavaBoard {
public:
	LavaBoard(Matrix<char> input) {
		n_ = input.size();
		m_ = input[n_-1].size();
    board_ = Matrix<int>(n_, vector<int>(m_,0));
    cout << n_ << ", " << m_ << endl;
		for (int i = 0; i < n_; i++) {
			for (int j = 0; j < m_; j++) {
				board_[i][j] = input[i][j] - '0';
			}
		}

	}

    int part1(bool part2) const {
      priority_queue<Node, vector<Node>, Compare> pq;
      unordered_map<Node, int> seen;
      Node start(Point2d<int>(0,0), Right, 0, 0);
      Node start2(Point2d<int>(0,0), Down, 0, 0);
      pq.push(start);
      pq.push(start2);
      cout << "hello" << endl;
      while (!pq.empty()) {
        Node curr = pq.top();
        pq.pop();
        // cout << "current:" << curr.p << ", " << curr.curr_score << ", " << curr.d << ", " << curr.moves << endl;
        if (part2) {
          if (curr.p == Point2d<int>(m_-1, n_-1) && curr.moves >= 4) {
            // cout << "found" << curr.curr_score << endl;
            return curr.curr_score;
          }
        } else {
          if (curr.p == Point2d<int>(m_-1, n_-1)) {
            return curr.curr_score;
          }
        }
        list<Node> neighbors;
        if (part2) {
          neighbors = get_neighbors_part2(curr);
        } else {
          neighbors = get_neighbors(curr);
        }
        for (auto& n : neighbors) {
          if (part2) {
              while (valid_point(n, part2) && n.moves < 4) {
                n.curr_score += board_[n.p.y_][n.p.x_];
                n = move(n, n.d);
              }
            }
          if (valid_point(n, part2)) {
            n.curr_score += board_[n.p.y_][n.p.x_];
            if (seen.find(n) != seen.end()) {
              if (n.curr_score < seen[n]) {
                seen[n] = n.curr_score;
                pq.push(n);
                // cout << " neighbor:" << n.p << ", " << n.curr_score << ", " << n.d << ", " << n.moves << endl;
              }
            } else {
              seen[n] = n.curr_score;
              pq.push(n);
            //   cout << " neighbor:" << n.p << ", " << n.curr_score << ", " << n.d << ", " << n.moves << endl;
            }
            // pq.push(n);
          }
        }
      }
      return 0;
    }

    // size_t

private:
	Matrix<int> board_;
	int n_;
	int m_;
  list<Node> get_neighbors(const Node& curr) const {
      list<Node> result;
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
  list<Node> get_neighbors_part2(const Node& curr) const {
      list<Node> result;
      switch (curr.d) {
      case Up:
          result.push_back(move(curr, Up));
          if (curr.moves >= 4) {
            result.push_back(move(curr, Left));
            result.push_back(move(curr, Right));
          }
          break;
      case Down:
          result.push_back(move(curr, Down));
          if (curr.moves >= 4) {
            result.push_back(move(curr, Left));
            result.push_back(move(curr, Right));
          }
          break;
      case Left:
          result.push_back(move(curr, Left));
          if (curr.moves >= 4) {
            result.push_back(move(curr, Up));
            result.push_back(move(curr, Down));
          }
          break;
      case Right:
          result.push_back(move(curr, Right));
          if (curr.moves >= 4) {
            result.push_back(move(curr, Up));
            result.push_back(move(curr, Down));
          }
          break;
      }
      return result;
    }
    Node move(const Node& curr, Direction dir) const {
        Node res(curr.p, dir, curr.curr_score, 1);
        if (dir == curr.d) {
            res.moves = curr.moves+1;
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
    bool valid_point(const Node& curr, bool part2) const {
      if (part2) {
        if (curr.moves > 10) {
            return false;
        }
      } else {
        if (curr.moves > 3) {
            return false;
        }
      }
        return curr.p >= Point2d<int>(0, 0) && curr.p < Point2d<int>(m_, n_);
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
    u64int_t part1 = lava_board.part1(false);
    u64int_t part2 = lava_board.part1(true);

    // part 1
    cout << "part1: " << part1 << endl;
    // part 2
    cout << "part2: " << part2 << endl;
    return 0;
}