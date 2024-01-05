#include <ostream>
#include <sstream>

template <typename T>
struct Point2d {
    Point2d(T x_in, T y_in) : x_(x_in), y_(y_in) {}
    Point2d() {}
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
    bool operator!=(const Point2d& other) const { return !(*this == other); }
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
    os << "(" << other.x_ << "," << other.y_ << ")";  // endl;
    return os;
}

namespace std {
template <typename T>
struct hash<Point2d<T>> {
    auto operator()(const Point2d<T>& point) const -> size_t {
        const T& a = point.x_;
        const T& b = point.y_;
        return a >= b ? a * a + a + b : a + b * b;
    }
};
}  // namespace std

struct Point3d {
    Point3d(int x_in, int y_in, int z_in) : x_(x_in), y_(y_in), z_(z_in) {}
    Point3d(const std::string& string_in) {
        std::stringstream ss(string_in);
        char c;
        ss >> x_ >> c >> y_ >> c >> z_;
        // cout << x_ << " " << y_ << " " << z_ << endl;
    }
    Point3d() {}
    bool operator==(const Point3d& other) const {
        return x_ == other.x_ && y_ == other.y_ && z_ == other.z_;
    }
    int x_;
    int y_;
    int z_;
    friend std::ostream& operator<<(std::ostream& os, const Point3d& other);
};

std::ostream& operator<<(std::ostream& os, const Point3d& other) {
    os << "(" << other.x_ << "," << other.y_ << "," << other.z_
       << ")";  // endl;
    return os;
}

namespace std {
template <>
struct hash<Point3d> {
    auto operator()(const Point3d& point) const -> size_t {
        const int& a = point.x_;
        const int& b = point.y_;
        const int& c = point.z_;
        return (((((37 + a) * 7) + b) * 31) + c) * 3;
    }
};
}  // namespace std