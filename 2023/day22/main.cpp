#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <unordered_map>

#include "grids.hpp"
#include "input_reader.hpp"

using namespace std;

struct Brick {
    Brick(const string& string_in) {
        int ind = string_in.find('~');
        p0_ = Point3d(string_in.substr(0, ind));
        p1_ = Point3d(string_in.substr(ind + 1));
        if (p0_.z_ > p1_.z_) {
            swap(p0_, p1_);
        }
    }
    vector<Point3d> get_points() const {
        vector<Point3d> result;
        for (int x = p0_.x_; x <= p1_.x_; ++x) {
            for (int y = p0_.y_; y <= p1_.y_; ++y) {
                for (int z = p0_.z_; z <= p1_.z_; ++z) {
                    result.emplace_back(x, y, z);
                }
            }
        }
        return result;
    }
    bool operator==(const Brick& other) const {
        return p0_ == other.p0_ && p1_ == other.p1_;
    }

    Point3d p0_;
    Point3d p1_;
};

namespace std {
template <>
struct hash<Brick> {
    auto operator()(const Brick& brick) const -> size_t {
        const Point3d& a = brick.p0_;
        const Point3d& b = brick.p1_;
        size_t lhs = std::hash<Point3d>()(a);
        size_t rhs = std::hash<Point3d>()(b);
        return lhs ^ (rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2));
    }
};
}  // namespace std

class BrickField {
   public:
    BrickField(const vector<Brick>& bricks) : bricks_(bricks) {}

    void settle_bricks() {
        sort(
            bricks_.begin(), bricks_.end(),
            [](const Brick& a, const Brick& b) { return a.p0_.z_ < b.p0_.z_; });
        for (auto brick : bricks_) {
            cout << brick.p0_ << " " << brick.p1_ << endl;
        }
        for (int i = 0; i < bricks_.size(); ++i) {
            vector<Point3d> points = bricks_[i].get_points();
            // play tetris
            set<int> collided = collided_bricks(points);
            bool hit_ground = false;
            while (collided.empty() && !hit_ground) {
                for (auto& point : points) {
                    point.z_--;
                    if (point.z_ == 0) {
                        hit_ground = true;
                    }
                }
                collided = collided_bricks(points);
            }
            for (auto& point : points) {
                point.z_++;
                occupied_space_[point] = i;
            }
            if (!hit_ground) {
                for (auto b : collided) {
                    below_[i].insert(b);
                    above_[b].insert(i);
                }
            }
        }
        for (int i = 0; i < bricks_.size(); ++i) {
            cout << i << " " << bricks_[i].p0_ << " " << bricks_[i].p1_ << endl;
            cout << "below: ";
            for (auto b : below_[i]) {
                cout << b << " ";
            }
            cout << endl;
            cout << "above: ";
            for (auto b : above_[i]) {
                cout << b << " ";
            }
            cout << endl;
        }
    }

    int get_disintegratable_bricks() const {
        int result = 0;
        cout << "disintegratable bricks: " << endl;
        for (int i = 0; i < bricks_.size(); ++i) {
            bool disintegratable = true;
            for (auto above : above_.at(i)) {
                if (below_.at(above).size() == 1) {
                    disintegratable = false;
                }
            }
            if (disintegratable) {
                cout << " " << i << " " << bricks_[i].p0_ << " "
                     << bricks_[i].p1_ << endl;
                result++;
            }
        }
        return result;
    }

    int disintegrate(int i) const {
        int result = 0;
        queue<int> q;
        unordered_map<int, set<int>> below = below_;
        unordered_map<int, set<int>> above = above_;
        q.push(i);
        while (!q.empty()) {
            int n = q.size();
            result += n;
            while (n--) {
                int curr = q.front();
                q.pop();
                for (auto a : above.at(curr)) {
                    below[a].erase(curr);
                    if (below.at(a).empty()) {
                        q.push(a);
                    }
                }
            }
        }
        return result - 1;
    }

   private:
    vector<Brick> bricks_;
    unordered_map<int, set<int>> below_;
    unordered_map<int, set<int>> above_;
    unordered_map<Point3d, int> occupied_space_;
    set<int> collided_bricks(const vector<Point3d>& points) {
        set<int> result;
        for (auto& point : points) {
            if (occupied_space_.find(point) != occupied_space_.end()) {
                result.insert(occupied_space_[point]);
            }
        }
        return result;
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
    vector<string> input_data = input_reader::read_as_strings(filename);

    vector<Brick> bricks;
    for (auto& line : input_data) {
        bricks.emplace_back(Brick(line));
        cout << line << endl;
    }
    unsigned long long part1 = 0;
    unsigned long long part2 = 0;

    BrickField bf(bricks);
    bf.settle_bricks();
    part1 = bf.get_disintegratable_bricks();

    for (int i = 0; i < bricks.size(); ++i) {
        int ans = bf.disintegrate(i);
        cout << i << " " << ans << endl;
        part2 += ans;
    }

    // part 1
    cout << "part1: " << part1 << endl;
    // part 2
    cout << "part2: " << part2 << endl;
    return 0;
}