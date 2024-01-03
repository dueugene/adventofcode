#include <iostream>
#include <unordered_map>

#include "input_reader.hpp"

using namespace std;

template <typename T>
using Matrix = vector<vector<T>>;

class RockBoard {
public:
    RockBoard(Matrix<char>& input) : board_(input), board_begin_(input) {
        n_ = board_.size();
        m_ = board_[0].size();
    }

    void tilt_board(char dir = 'N') {
        if (dir == 'N') {
            for (int j = 0; j < m_; j++) {
                int anchor = -1;
                for (int i = 0; i < n_; i++) {
                    switch (board_[i][j]) {
                    case '#':
                        anchor = i;
                        break;
                    case 'O':
                        board_[i][j] = '.';
                        anchor++;
                        board_[anchor][j] = 'O';
                        break;
                    case '.':
                        // do nothing
                        break;
                    }
                }
            }
        } else if (dir == 'S') {
            for (int j = 0; j < m_; j++) {
                int anchor = n_;
                for (int i = n_ - 1; i >= 0; i--) {
                    switch (board_[i][j]) {
                    case '#':
                        anchor = i;
                        break;
                    case 'O':
                        board_[i][j] = '.';
                        anchor--;
                        board_[anchor][j] = 'O';
                        break;
                    case '.':
                        // do nothing
                        break;
                    }
                }
            }
        } else if (dir == 'W') {
            for (int i = 0; i < n_; i++) {
                int anchor = -1;
                for (int j = 0; j < m_; j++) {
                    switch (board_[i][j]) {
                    case '#':
                        anchor = j;
                        break;
                    case 'O':
                        board_[i][j] = '.';
                        anchor++;
                        board_[i][anchor] = 'O';
                        break;
                    case '.':
                        // do nothing
                        break;
                    }
                }
            }
        } else if (dir == 'E') {
            for (int i = 0; i < n_; i++) {
                int anchor = m_;
                for (int j = m_ - 1; j >= 0; j--) {
                    switch (board_[i][j]) {
                    case '#':
                        anchor = j;
                        break;
                    case 'O':
                        board_[i][j] = '.';
                        anchor--;
                        board_[i][anchor] = 'O';
                        break;
                    case '.':
                        // do nothing
                        break;
                    }
                }
            }
        }
    }

    void spin_cycle() {
        tilt_board('N');
        tilt_board('W');
        tilt_board('S');
        tilt_board('E');
    }

    int calc_load() {
        int load = 0;
        for (int i = 0; i < n_; i++) {
            for (int j = 0; j < m_; j++) {
                if (board_[i][j] == 'O') {
                    load += (n_-i);
                }
            }
        }
        return load;
    }

    string curr_board_as_str() const {
        string res;
        for (int i = 0; i < n_; i++) {
            for (int j = 0; j < m_; j++) {
                res.push_back(board_[i][j]);
            }
        }
        return res;
    }
    void reset() {
        board_ = board_begin_;
    }
    friend ostream& operator<<(ostream& os, const RockBoard& other);
private:
    Matrix<char> board_;
    Matrix<char> board_begin_;
    int n_;
    int m_;
};

ostream& operator<<(ostream& os, const RockBoard& other) {
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

    RockBoard rock_board(input_data);
    cout << rock_board << endl;
    rock_board.tilt_board();
    cout << rock_board << endl;
    ulong part1 = rock_board.calc_load();

    rock_board.reset();
    unordered_map<string, pair<int, int>> spin_cycle_map;
    // unordered_set<string> seen_boards;
    // list<string> order;
    unordered_map<int, int> imap;
    ulong part2 =  0;
    for (int i = 0; i < 1000000; i++) {
        rock_board.spin_cycle();
        int res = rock_board.calc_load();
        string key = rock_board.curr_board_as_str();
        if (spin_cycle_map.find(key) != spin_cycle_map.end()) {
            auto p = spin_cycle_map[key];
            int first_occurance = p.first;
            int n = spin_cycle_map.size() - first_occurance;
            for (auto b : spin_cycle_map) {
                cout << b.first << ": " << b.second.first << ", " << b.second.second << endl;
            }
            cout << spin_cycle_map.size() << " " << first_occurance << endl;
            cout << (1000000000-first_occurance+1) % n << endl;
            cout <<(1000000000-first_occurance) % n << endl;
            part2 = imap[((1000000000-first_occurance-1) % n) + first_occurance];

            break;
        }
        spin_cycle_map[key] =  {i, res};
        imap[i] = res;
    }
    // part 1
    cout << "part1: " << part1 << endl;
    // part 2
    cout << "part2: " << part2 << endl;
    return 0;
}