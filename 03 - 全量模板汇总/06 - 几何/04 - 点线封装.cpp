template<class T> struct Point { // 在C++17下使用 emplace_back 绑定可能会导致CE！
    T x, y;
    Point(T x_ = 0, T y_ = 0) : x(x_), y(y_) {} // 初始化
    template<class U> operator Point<U>() { // 自动类型匹配
        return Point<U>(U(x), U(y));
    }
    Point &operator+=(Point p) & { return x += p.x, y += p.y, *this; }
    Point &operator+=(T t) & { return x += t, y += t, *this; }
    Point &operator-=(Point p) & { return x -= p.x, y -= p.y, *this; }
    Point &operator-=(T t) & { return x -= t, y -= t, *this; }
    Point &operator*=(T t) & { return x *= t, y *= t, *this; }
    Point &operator/=(T t) & { return x /= t, y /= t, *this; }
    Point operator-() const { return Point(-x, -y); }
    friend Point operator+(Point a, Point b) { return a += b; }
    friend Point operator+(Point a, T b) { return a += b; }
    friend Point operator-(Point a, Point b) { return a -= b; }
    friend Point operator-(Point a, T b) { return a -= b; }
    friend Point operator*(Point a, T b) { return a *= b; }
    friend Point operator*(T a, Point b) { return b *= a; }
    friend Point operator/(Point a, T b) { return a /= b; }
    friend bool operator<(Point a, Point b) {
        return equal(a.x, b.x) ? a.y < b.y - EPS : a.x < b.x - EPS;
    }
    friend bool operator>(Point a, Point b) { return b < a; }
    friend bool operator==(Point a, Point b) { return !(a < b) && !(b < a); }
    friend bool operator!=(Point a, Point b) { return a < b || b < a; }
    friend auto &operator>>(istream &is, Point &p) {
        return is >> p.x >> p.y;
    }
    friend auto &operator<<(ostream &os, Point p) {
        return os << "(" << p.x << ", " << p.y << ")";
    }
};
template<class T> struct Line {
    Point<T> a, b;
    Line(Point<T> a_ = Point<T>(), Point<T> b_ = Point<T>()) : a(a_), b(b_) {}
    template<class U> operator Line<U>() { // 自动类型匹配
        return Line<U>(Point<U>(a), Point<U>(b));
    }
    friend auto &operator<<(ostream &os, Line l) {
        return os << "<" << l.a << ", " << l.b << ">";
    }
};
