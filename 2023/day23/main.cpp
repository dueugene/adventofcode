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

enum Direction { Up, Down, Left, Right };

struct Node {
    Node(Point2d<int> p_in, Direction d_in, size_t curr_score_in, size_t moves_in)
        : p(p_in), d(d_in), curr_score(curr_score_in), moves(moves_in) {}
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
    auto operator()(const Node& point) const -> size_t {
        size_t lhs = hash<Point2d<int>>()(point.p);
        size_t rhs = point.d;
        size_t moves = point.moves;
        lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
        lhs ^= moves + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
        return lhs;
    }
};
}  // namespace std

class Maze {
   public:
    Maze(const vector<string>& maze) : maze_(maze) {
        n_ = maze_.size();
        m_ = maze_[0].size();
    }
    int longest_hike() const {
        int result = 0;
        Point2d<int> start, end;
        for (int j = 0; j < m_; ++j) {
            if (maze_[0][j] == '.') {
                start = Point2d<int>(j, 0);
            }
            if (maze_[n_ - 1][j] == '.') {
                end = Point2d<int>(j, n_ - 1);
            }
        }
        cout << start << " " << end << endl;
        unordered_set<Point2d<int>> visited;
        dfs(start, end, visited, 0, result);
        return result;
    }

    int longest_hike2() const {
        Point2d<int> start, end;
        for (int j = 0; j < m_; ++j) {
            if (maze_[0][j] == '.') {
                start = Point2d<int>(j, 0);
            }
            if (maze_[n_ - 1][j] == '.') {
                end = Point2d<int>(j, n_ - 1);
            }
        }
        cout << start << " " << end << endl;
        // convert the maze to a graph of nodes
        // create a node at each "fork" in the maze
        // list<Point2d<int>> nodes;
        size_t node_count = 0;
        unordered_map<Point2d<int>, int> nodes;
        vector<Point2d<int>> node_vals;
        node_vals.push_back(start);
        node_vals.push_back(end);
        nodes[start] = node_count++;
        nodes[end] = node_count++;
        queue<Node> q;
        int n_nodes_max = 1000;  // estimation
        vector<vector<int>> adj_list(n_nodes_max, vector<int>(n_nodes_max, -1));
        q.push({start, Down, 0, 0});
        while (!q.empty()) {
            auto current = q.front();
            q.pop();
            auto neighbors = get_all_neighbors(current.p, current.d);
            vector<Point2d<int>> valid_neighbors;
            for (auto next : neighbors) {
                if (nodes.find(next) != nodes.end()) {
                    // neighbor is a node that we already visited
                    adj_list[current.curr_score][nodes[next]] = current.moves + 1;
                    adj_list[nodes[next]][current.curr_score] = current.moves + 1;
                } else if (next >= Point2d<int>(0, 0) && next <= Point2d<int>(m_ - 1, n_ - 1) &&
                           maze_[next.y_][next.x_] != '#') {
                    valid_neighbors.push_back(next);
                }
            }
            if (valid_neighbors.size() > 1) {
                if (nodes.find(current.p) == nodes.end()) {
                    // found a new fork
                    cout << "found a new fork" << endl;
                    cout << " i" << node_count << " " << current.p << " " << current.moves << endl;
                    nodes[current.p] = node_count;
                    // use curr_score to store the node id
                    adj_list[current.curr_score][node_count] = current.moves;
                    adj_list[node_count][current.curr_score] = current.moves;
                    for (auto next : valid_neighbors) {
                        if (nodes.find(next) == nodes.end()) {
                            q.push({next, current.d, node_count, 1});
                        }
                    }
                    node_vals.push_back(current.p);
                    node_count++;
                }
            } else {
                // keep going
                if (valid_neighbors.size() == 1) {
                    auto next = valid_neighbors[0];
                    auto dire = next - current.p;
                    if (dire == directions[Up]) {
                        q.push({valid_neighbors[0], Up, current.curr_score, current.moves + 1});
                    } else if (dire == directions[Down]) {
                        q.push({valid_neighbors[0], Down, current.curr_score, current.moves + 1});
                    } else if (dire == directions[Left]) {
                        q.push({valid_neighbors[0], Left, current.curr_score, current.moves + 1});
                    } else if (dire == directions[Right]) {
                        q.push({valid_neighbors[0], Right, current.curr_score, current.moves + 1});
                    }
                }
            }
        }
        for (int i = 0; i < node_count; ++i) {
            cout << i << ": " << node_vals[i] << endl;
            for (int j = 0; j < node_count; ++j) {
                if (adj_list[i][j] != -1) {
                    cout << j << " " << adj_list[i][j] << ", ";
                }
            }
            cout << endl;
        }

        // find the longest path in the graph
        int result;
        set<int> visited;
        dfs2(0, 1, visited, adj_list, 0, result);

        return result;
    }

   private:
    void dfs2(int current, int end, set<int>& visited, const vector<vector<int>>& adj_list, int current_length,
              int& result) const {
        if (current == end) {
            cout << current_length << endl;
            result = max(result, current_length);
            return;
        }
        visited.insert(current);
        // cout << current << endl;
        for (int i = 0; i < adj_list.size(); ++i) {
            if (current != i && adj_list[current][i] != -1 && visited.find(i) == visited.end()) {
                // cout << " " << next << endl;
                dfs2(i, end, visited, adj_list, current_length + adj_list[current][i], result);
            }
        }
        visited.erase(current);
    }

    void dfs(const Point2d<int>& current, const Point2d<int>& end, unordered_set<Point2d<int>>& visited,
             int current_length, int& result) const {
        if (current == end) {
            cout << current_length << endl;
            result = max(result, current_length);
            return;
        }
        visited.insert(current);
        // cout << current << endl;
        for (auto next : get_neighbors(current)) {
            if (next >= Point2d<int>(0, 0) && next <= Point2d<int>(m_ - 1, n_ - 1) && maze_[next.y_][next.x_] != '#' &&
                visited.find(next) == visited.end()) {
                // cout << " " << next << endl;
                dfs(next, end, visited, current_length + 1, result);
            }
        }
        visited.erase(current);
    }
    // enum Direction {Up, Down, Left, Right};
    vector<Point2d<int>> directions = {Point2d<int>(0, -1), Point2d<int>(0, 1), Point2d<int>(-1, 0),
                                       Point2d<int>(1, 0)};

    list<Point2d<int>> get_all_neighbors(const Point2d<int>& current, Direction dir) const {
        list<Point2d<int>> result;
        switch (dir) {
            case Up:
                result.push_back(current + directions[Up]);
                result.push_back(current + directions[Left]);
                result.push_back(current + directions[Right]);
                break;
            case Down:
                result.push_back(current + directions[Down]);
                result.push_back(current + directions[Left]);
                result.push_back(current + directions[Right]);
                break;
            case Left:
                result.push_back(current + directions[Up]);
                result.push_back(current + directions[Down]);
                result.push_back(current + directions[Left]);
                break;
            case Right:
                result.push_back(current + directions[Up]);
                result.push_back(current + directions[Down]);
                result.push_back(current + directions[Right]);
                break;
        }
        return result;
    }

    list<Point2d<int>> get_neighbors(const Point2d<int>& current) const {
        list<Point2d<int>> result;
        switch (maze_[current.y_][current.x_]) {
            case '>':
                result.push_back(current + directions[Right]);
                break;
            case '<':
                result.push_back(current + directions[Left]);
                break;
            case 'v':
                result.push_back(current + directions[Down]);
                break;
            case '^':
                result.push_back(current + directions[Up]);
                break;
            case '.':
                result.push_back(current + directions[Up]);
                result.push_back(current + directions[Down]);
                result.push_back(current + directions[Left]);
                result.push_back(current + directions[Right]);
                break;
            default:
                break;
        }
        return result;
    }
    vector<string> maze_;
    int n_;
    int m_;
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
    vector<string> input_data = input_reader::read_as_strings(filename);

    unsigned long long part1 = 0;
    unsigned long long part2 = 0;
    Maze maze(input_data);
    part1 = maze.longest_hike();
    part2 = maze.longest_hike2();

    // part 1
    cout << "part1: " << part1 << endl;
    // part 2
    cout << "part2: " << part2 << endl;
    return 0;
}