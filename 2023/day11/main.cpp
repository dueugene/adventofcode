#include <iostream>
#include <vector>
#include <map>

#include "input_reader.hpp"
#include "grids.hpp"

using namespace std;

template <typename T>
using Matrix = vector<vector<T>>;
using ulong = unsigned long long;

class ObservationMap {
public:
    ObservationMap(const Matrix<char>& input) {
        n_ = input.size();
        m_ = input[0].size();
        contains_galaxy_x_ = vector<bool>(m_, false);
        contains_galaxy_y_ = vector<bool>(n_, false);
        for (long long int i = 0; i < n_; i++) {
            for (long long int j = 0; j < m_; j++) {
                if (input[i][j] == '#') {
                    galaxies_.push_back(Point2d(j, i));
                    contains_galaxy_x_[j] = true;
                    contains_galaxy_y_[i] = true;
                }
            }
        }
        galaxies_init_ = galaxies_;
    }
    void expand_galaxies(int expansion_coeff) {
        map<int, int, greater<int>> expansion_x;
        map<int, int, greater<int>> expansion_y;
        expansion_coeff--;
        int count = 0;
        for (int i = 0; i < n_; i++) {
            if (contains_galaxy_y_[i] == false) {
                count += expansion_coeff;
                expansion_y.insert({i, count});
            }
        }
        count = 0;
        for (int j = 0; j < m_; j++) {
            if (contains_galaxy_x_[j] == false) {
                count += expansion_coeff;
                expansion_x.insert({j, count});
            }
        }
        cout << "x:" << endl;
        for (auto p : expansion_x) {
            cout << p.first << ": " << p.second << endl;
        }
        cout << "y:" << endl;
        for (auto p : expansion_y) {
            cout << p.first << ": " << p.second << endl;
        }
        for (auto& g : galaxies_) {
            auto it = expansion_x.lower_bound(g.x_);
            if (it != expansion_x.end()) {
                auto t = g;
                t.x_ += it->second;
                // cout << g << "->" << t << endl;
                g.x_ += it->second;
            }
            auto it2 = expansion_y.lower_bound(g.y_);
            if (it2 != expansion_y.end()) {
                auto t = g;
                t.y_ += it2->second;
                // cout << g << "->" << t << endl;
                g.y_ += it2->second;
            }
            // cout << endl;
        }
    }
    void reset() {
         galaxies_ = galaxies_init_;
    }
    ulong sum_distance_between_galaxies() {
        ulong result = 0;
        for (int i = 0; i < galaxies_.size(); i++) {
            for (int j = i + 1; j < galaxies_.size(); j++) {
                // cout << i << ": " << galaxies_[i] << "->" << galaxies_[j] << ": ";
                ulong dist = abs(galaxies_[i].x_ - galaxies_[j].x_) + abs(galaxies_[i].y_ - galaxies_[j].y_);
                // cout << dist << endl;
                result += dist;
            }
            // EUGENE
        }
        return result;
    }
private:
    vector<Point2d<long long int>> galaxies_;
    vector<Point2d<long long int>> galaxies_init_;
    int n_;
    int m_;
    vector<bool> contains_galaxy_x_;
    vector<bool> contains_galaxy_y_;
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
    
    ObservationMap galaxy_map(input_data);
    galaxy_map.reset();
    galaxy_map.expand_galaxies(2);
    ulong part1 = galaxy_map.sum_distance_between_galaxies();
    galaxy_map.reset();
    galaxy_map.expand_galaxies(1000000);
    ulong part2 = galaxy_map.sum_distance_between_galaxies();

    // part 1
    cout << "part1: " << part1 << endl;
    // part 2
    cout << "part2: " << part2 << endl;
    return 0;
}