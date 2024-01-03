#include <ostream>

template <typename T>
struct Point2d {
    Point2d(T x_in, T y_in) : x_(x_in), y_(y_in) {

    }
    Point2d() {

    }
    T x_;
    T y_;
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
    template <typename B>
    friend std::ostream& operator<<(std::ostream& os, const Point2d<B>& other);
};

template <typename B>
std::ostream& operator<<(std::ostream& os, const Point2d<B>& other) {
    os << "(" << other.x_ << "," << other.y_ << ")"; // endl;
    return os;
}

namespace std {
template <typename T>
struct hash<Point2d<T>> {
  auto operator()(const Point2d<T> &point) const -> size_t {
    const T& a = point.x_;
    const T& b = point.y_;
    return a >= b ? a * a + a + b : a + b * b;
  }
};
}  // namespace std