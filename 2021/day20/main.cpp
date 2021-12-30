#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>
#include <queue>
#include <unordered_set>

#include "../utils/file_read.cpp"
#include "../utils/hash.cpp"
#include "../utils/misc.cpp"

using namespace std;

class EnhancedImage {
public:
    EnhancedImage(vector<string> grid, string key) {
        enhance_count_ = 0;
        key_ = key;
        n_ = grid.size();
        m_ = grid[0].size();
        for (int i = 0; i < n_; i++) {
            for (int j = 0; j < m_; j++) {
                if (grid[i][j] == '#') {
                    coords_.insert({j, i});
                }
            }
        }
    };

    int enhance() {
        enhance_count_++;
        unordered_set<pair<int, int>, PairHasher> prev = coords_;
        coords_.clear();
        for (int i = -enhance_count_; i < n_ + enhance_count_; i++) {
            for (int j = -enhance_count_; j < m_ + enhance_count_; j++) {
                string bin_string(9,'0');
                int ind = 0;
                for (int k = i - 1; k < i + 2; k++) {
                    for (int l = j - 1; l < j + 2; l++) {
                        if (prev.find({l, k}) != prev.end()) {
                            bin_string[ind] = '1';
                        } else {
                            bin_string[ind] = '0';
                        }
                        ind++;
                    }
                }
                char c = key_[bin_string_to_int(bin_string)];
                if (c == '#') {
                    coords_.insert({j, i});
                }
            }
        }

        // unique to our input values, the pixels passed our borders to infinity flickr on/off
        // the current algorithm coouldn't account for infinite pixels, but we can paint the border
        // every other iteration
        // I have hard coded here, but we can deduce this "blinking" behaviour based on key_ string
        if (enhance_count_ % 2 == 1) {
            for (int k = 1; k < 3; k++) {
                for (int i = -enhance_count_-k; i < n_ + enhance_count_+k; i++) {
                    coords_.insert({-enhance_count_-k, i});
                    coords_.insert({m_ + enhance_count_+k-1, i});
                }
                for (int j = -enhance_count_-k; j < m_ + enhance_count_+k; j++) {
                    coords_.insert({j, -enhance_count_-k});
                    coords_.insert({j, n_ + enhance_count_+k-1});
                }
            }
        }
        
        return coords_.size();
    };

    void print() {
        for (int i = -enhance_count_-2; i < n_ + enhance_count_+2; i++) {
            for (int j = -enhance_count_-2; j < m_ + enhance_count_+2; j++) {
                if (coords_.find({j, i}) != coords_.end()) {
                    cout << '#';
                } else {
                    cout << '.';
                }
            }
            cout << endl;
        }
    }
private:
    int n_;
    int m_;
    int enhance_count_;
    string key_;
    unordered_set<pair<int, int>, PairHasher> coords_;
};

int main(int argc, char** argv) {
    // this will allow different input files to be passed
    string filename;
    if (argc > 1) {
        filename = argv[1];
    } else {
        // default input
        filename = "sample.txt";
    }

    // read input
    vector<string> grid = read_input(filename);
    string key = grid[0];
    grid.erase(grid.begin());
    grid.erase(grid.begin());
    for (auto l : grid) {
        cout << l << endl;
    }
   
    // part 1
    EnhancedImage image(grid, key);
    image.print();
    image.enhance();
    cout << "part1: " << image.enhance() << endl;
    // part 2
    for (int i = 3; i < 50; i++) {
        image.enhance();
    }
    cout << "part2: " << image.enhance() << endl;

    return 0;
}