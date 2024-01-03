#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "input_reader.hpp"

using namespace std;

struct Point2d {
    Point2d(int x_, int y_) : x(x_), y(y_) {}
    int x;  // row
    int y;  // col
    bool operator==(const Point2d& rhs) const {
        return x == rhs.x && y == rhs.y;
    }
    bool operator<(const Point2d& rhs) const {
        return x < rhs.x || (x == rhs.x && y < rhs.y);
    }
};

class Mirrors {
   public:
    Mirrors() {}
    Mirrors(vector<Point2d> points) : points_(points) {
        for (auto& p : points_) {
            n = max(n, p.x);
            m = max(m, p.y);
        }
    }
    friend ostream& operator<<(ostream& os, const Mirrors& m);

    int find_reflection() {
        for (int j = 0; j < m; j++) {
            set<Point2d> a;
            set<Point2d> b;
            int m_max = min(m - j, -(-j - 1));
            // cout << "j: " << j << " m_max: " << m_max << endl;
            for (auto p : points_) {
                if (p.y <= j) {
                    p.y = -(p.y - j - 1);
                    if (p.y <= m_max) {
                        a.insert(p);
                    }
                } else {
                    p.y = p.y - j;
                    if (p.y <= m_max) {
                        b.insert(p);
                    }
                }
            }
            if (compare_hori(a, b)) {
                return (j + 1);
            }
        }
        for (int i = 0; i < n; i++) {
            set<Point2d> a;
            set<Point2d> b;
            int n_max = min(n - i, -(-i - 1));
            for (auto p : points_) {
                if (p.x <= i) {
                    p.x = -(p.x - i - 1);
                    if (p.x <= n_max) {
                        a.insert(p);
                    }
                } else {
                    p.x = p.x - i;
                    if (p.x <= n_max) {
                        b.insert(p);
                    }
                }
            }
            if (compare_hori(a, b)) {
                return 100 * (i + 1);
            }
        }
        return 0;
    }

    int find_smudge() {
        int reflection_score = find_reflection();
        for (int j = 0; j < m; j++) {
            set<Point2d> a;
            set<Point2d> b;
            int m_max = min(m - j, -(-j - 1));
            // cout << "j: " << j << " m_max: " << m_max << endl;
            for (auto p : points_) {
                if (p.y <= j) {
                    p.y = -(p.y - j - 1);
                    if (p.y <= m_max) {
                        a.insert(p);
                    }
                } else {
                    p.y = p.y - j;
                    if (p.y <= m_max) {
                        b.insert(p);
                    }
                }
            }
            if (compare_for_smudge(a, b)) {
                return (j + 1);
            }
        }
        for (int i = 0; i < n; i++) {
            set<Point2d> a;
            set<Point2d> b;
            int n_max = min(n - i, -(-i - 1));
            for (auto p : points_) {
                if (p.x <= i) {
                    p.x = -(p.x - i - 1);
                    if (p.x <= n_max) {
                        a.insert(p);
                    }
                } else {
                    p.x = p.x - i;
                    if (p.x <= n_max) {
                        b.insert(p);
                    }
                }
            }
            if (compare_for_smudge(a, b)) {
                return 100 * (i + 1);
            }
        }
        return 0;
    }

   private:
    bool compare_for_smudge(set<Point2d>& a, set<Point2d>& b) {
        set<Point2d>& a2 = a.size() > b.size() ? a : b;
        set<Point2d>& b2 = a.size() > b.size() ? b : a;
        if (a2.size() - b2.size() != 1) {
            return false;
        }
        for (auto& p : b2) {
            if (a2.find(p) == a2.end()) {
                return false;
            }
        }
        return true;
    }
    bool compare_hori(set<Point2d>& a, set<Point2d>& b) {
        if (a.size() != b.size()) {
            return false;
        }
        for (auto& p : a) {
            if (b.find(p) == b.end()) {
                return false;
            }
        }
        return true;
    }
    vector<Point2d> points_;
    int n = 0;
    int m = 0;
};

ostream& operator<<(ostream& os, const Mirrors& m) {
    for (auto& p : m.points_) {
        os << "(" << p.x << "," << p.y << ") ";
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
    vector<string> lines = input_reader::read_as_strings(filename);
    vector<Mirrors> mirrors;
    vector<Point2d> points;
    int row = 0;
    for (auto& line : lines) {
            cout << line << endl;
            if (!line.empty()) {
                for (int col = 0; col < line.size(); ++col) {
                    if (line[col] == '#') {
                        points.push_back(Point2d(row, col));
                    }
                }
                row++;
            } else {
                row = 0;
                mirrors.push_back(Mirrors(points));
                points.clear();
            }
        }
    mirrors.push_back(Mirrors(points));

    for (auto& m : mirrors) {
        cout << m << endl;
    }

    long long int part1 = 0;
    vector<int> prev_reflections;
    for (auto& m : mirrors) {
        cout << m.find_reflection() << endl;
        prev_reflections.push_back(m.find_reflection());
        part1 += m.find_reflection();
    }
    cout << "part1: " << part1 << endl;

    // part 2
    ulong part2 = 0;
    for (auto& m : mirrors) {
        part2 += m.find_smudge();
    }
    cout << "part2: " << part2 << endl;

    return 0;
}
