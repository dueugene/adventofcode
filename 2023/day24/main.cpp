#include <iostream>

#include "grids.hpp"
#include "input_reader.hpp"

using namespace std;

enum Direction { Up, Down, Left, Right };

// line representing y = mx + b
class Line {
   public:
    Line(const string& input_str) {
        istringstream ss(input_str);
        long long int num;
        vector<long long int> nums;
        char c;
        while (ss >> num) {
            ss >> c;
            nums.push_back(num);
            // cout << num << endl;
        }
        m_ = (double)(nums[4]) / (double)(nums[3]);
        b_ = (double)(nums[1]) - m_ * (double)(nums[0]);
        p1_ = Point2d<double>(nums[0], nums[1]);
        positive_x = nums[3] > 0;
        positive_y = nums[4] > 0;
    }

    bool get_y(double x, Point2d<double>& result) const {
        if (positive_x && x < p1_.x_) {
            return false;
        } else if (!positive_x && x > p1_.x_) {
            return false;
        }
        double y = m_ * x + b_;
        if (positive_y && y < p1_.y_) {
            return false;
        } else if (!positive_y && y > p1_.y_) {
            return false;
        }
        result = Point2d<double>(x, y);
        return true;
    }
    friend bool get_intersection(const Line& l1, const Line& l2, Point2d<double>& result, Point2d<double> min_point,
                                 Point2d<double> max_point);

   private:
    double m_;
    double b_;
    Point2d<double> p1_;
    bool positive_x;
    bool positive_y;
};

bool get_intersection(const Line& l1, const Line& l2, Point2d<double>& result, Point2d<double> min_point,
                      Point2d<double> max_point) {
    if (l1.m_ == l2.m_) {
        // paralel lines
        cout << "paralel lines" << endl;
        if (l1.b_ != l2.b_) {
            // non congruent lines
            return false;
        }
        Point2d<double> p_intersect;
        if (l1.get_y(min_point.x_, p_intersect) && l2.get_y(min_point.x_, p_intersect)) {
            return true;
        }
        if (l1.get_y(max_point.x_, p_intersect) && l2.get_y(max_point.x_, p_intersect)) {
            return true;
        }

        return false;
    }
    double x = (l2.b_ - l1.b_) / (l1.m_ - l2.m_);
    Point2d<double> p_intersect(x, 0);
    if (!(l1.get_y(x, p_intersect) && l2.get_y(x, p_intersect))) {
        //
        return false;
    }
    result = p_intersect;
    if (!(result >= min_point && result <= max_point)) {
        return false;
    }
    return true;
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
    vector<string> input_data = input_reader::read_as_strings(filename);

    vector<Line> lines;
    for (auto& line : input_data) {
        // cout << line << endl;
        lines.push_back(Line(line));
    }
    Point2d<double> min_point(7, 7);
    Point2d<double> max_point(27, 27);
    // use below lines for actual input
    // Point2d<double> min_point(200000000000000.0, 200000000000000.0);
    // Point2d<double> max_point(400000000000000.0, 400000000000000.0);

    unsigned long long part1 = 0;
    unsigned long long part2 = 0;
    // part 1
    for (int i = 0; i < lines.size(); ++i) {
        for (int j = i + 1; j < lines.size(); ++j) {
            Point2d<double> result;
            if (get_intersection(lines[i], lines[j], result, min_point, max_point)) {
                cout << "intersection: " << result << i << " " << j << endl;
                part1++;
            } else {
                cout << "no intersection" << i << " " << j << endl;
            }
        }
    }

    cout << "part1: " << part1 << endl;
    // part 2
    // could not get part2, but I saw some interesting solutions online
    // some important facts:
    // system is overdetermined, so you do not need all the data
    // you may not be able to setup a linear equation system, but you can do some "guess and check"
    // or some gradient descent methods
    cout << "part2: " << part2 << endl;
    return 0;
}