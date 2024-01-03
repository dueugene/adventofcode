#include <ostream>

struct Point2d {
    Point2d(int x_in, int y_in) : x_(x_in), y_(y_in) {

    }
    Point2d() {

    }
    int x_;
    int y_;
    bool operator<=(const Point2d& other) const {
        return this->x_ <= other.x_ && this->y_ <= other.y_;
    }
    bool operator<(const Point2d& other) const {
        return this->x_ < other.x_ && this->y_ < other.y_;
    }
    bool operator>=(const Point2d& other) const {
        return this->x_ >= other.x_ && this->y_ >= other.y_;
    }
    bool operator==(const Point2d& other) const {
        return this->x_ == other.x_ && this->y_ == other.y_;
    }
    bool operator!=(const Point2d& other) const {
        return !(*this == other);
    }
    Point2d operator+(const Point2d& other) const {
        Point2d result(*this);
        result.x_ += other.x_;
        result.y_ += other.y_;
        return result;
    }
    Point2d operator-(const Point2d& other) const {
        Point2d result(*this);
        result.x_ -= other.x_;
        result.y_ -= other.y_;
        return result;
    }
    friend std::ostream& operator<<(std::ostream& os, const Point2d& other);
};
std::ostream& operator<<(std::ostream& os, const Point2d& other) {
    os << "(" << other.x_ << "," << other.y_ << ")"; // endl;
    return os;
}

namespace std {
template <>
struct hash<Point2d> {
  auto operator()(const Point2d &point) const -> size_t {
    const int& a = point.x_;
    const int& b = point.y_;
    return a >= b ? a * a + a + b : a + b * b;
  }
};
}  // namespace std