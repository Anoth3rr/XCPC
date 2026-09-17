using ld = long double;

template <class T> struct Point {
    T x;
    T y;
    Point(const T &_x = 0, const T &_y = 0) : x(_x), y(_y) {}

    template <class U> operator Point<U>() {
        return Point<U>(U(x), U(y));
    }
    Point &operator+=(const Point &p) & {
        x += p.x;
        y += p.y;
        return *this;
    }
    Point &operator-=(const Point &p) & {
        x -= p.x;
        y -= p.y;
        return *this;
    }
    Point &operator*=(const T &v) & {
        x *= v;
        y *= v;
        return *this;
    }
    Point &operator/=(const T &v) & {
        x /= v;
        y /= v;
        return *this;
    }
    Point operator-() const {
        return Point(-x, -y);
    }
    friend Point operator+(Point a, const Point &b) {
        return a += b;
    }
    friend Point operator-(Point a, const Point &b) {
        return a -= b;
    }
    friend Point operator*(Point a, const T &b) {
        return a *= b;
    }
    friend Point operator/(Point a, const T &b) {
        return a /= b;
    }
    friend Point operator*(const T &a, Point b) {
        return b *= a;
    }
    friend bool operator==(const Point &a, const Point &b) {
        return a.x == b.x && a.y == b.y;
    }
    friend bool operator!=(const Point &a, const Point &b) {
        return !(a == b);
    }
    friend istream &operator>>(istream &is, Point &p) {
        return is >> p.x >> p.y;
    }
    friend ostream &operator<<(ostream &os, const Point &p) {
        return os << "(" << p.x << ", " << p.y << ")";
    }
};

template <class T> struct Line {
    Point<T> a;
    Point<T> b;
    Line(const Point<T> &_a = Point<T>(), const Point<T> &_b = Point<T>()) : a(_a), b(_b) {}
};

template <class T> T dot(const Point<T> &a, const Point<T> &b) {
    return a.x * b.x + a.y * b.y;
}

template <class T> T cross(const Point<T> &a, const Point<T> &b) {
    return a.x * b.y - a.y * b.x;
}

template <class T> T square(const Point<T> &p) {
    return dot(p, p);
}

template <class T> double length(const Point<T> &p) {
    return sqrt(square(p));
}

template <class T> double length(const Line<T> &l) {
    return length(l.a - l.b);
}

template <class T> Point<T> norm(const Point<T> &p) {
    return p / length(p);
}

template <class T> bool isPara(const Line<T> &l1, const Line<T> &l2) {
    return cross(l1.b - l1.a, l2.b - l2.a) == 0;
}

template <class T> double dis(const Point<T> &a, const Point<T> &b) {
    return length(a - b);
}

template <class T> double disPL(const Point<T> &p, const Line<T> &l) {
    return abs(cross(l.a - l.b, l.a - p)) / length(l);
}

template <class T> double disPS(const Point<T> &p, const Line<T> &l) {
    if (dot(p - l.a, l.b - l.a) < 0) {
        return dis(p, l.a);
    }
    if (dot(p - l.b, l.a - l.b) < 0) {
        return dis(p, l.b);
    }
    return disPL(p, l);
}

template <class T> Point<T> rotate(const Point<T> &a) {
    return Point(-a.y, a.x);
}

template <class T> int sgn(const Point<T> &a) {
    return a.y > 0 || (a.y == 0 && a.x > 0) ? 1 : -1;
}

template <class T> bool side(const Point<T> &p, const Line<T> &l) {
    return cross(l.b - l.a, p - l.a) > 0;
}

template <class T> Point<T> inter(const Line<T> &l1, const Line<T> &l2) {
    return l1.a + (l1.b - l1.a) * (cross(l2.b - l2.a, l1.a - l2.a) / cross(l2.b - l2.a, l1.a - l1.b));
}

template <class T> bool onSeg(const Point<T> &p, const Line<T> &l) {
    return cross(p - l.a, l.b - l.a) == 0 && min(l.a.x, l.b.x) <= p.x && p.x <= max(l.a.x, l.b.x) && min(l.a.y, l.b.y) <= p.y && p.y <= max(l.a.y, l.b.y);
}

template <class T> bool inPoly(const Point<T> &a, const vector<Point<T>> &p) {
    int n = p.size();
    for (int i = 0; i < n; i++) {
        if (onSeg(a, Line(p[i], p[(i + 1) % n]))) {
            return true;
        }
    }

    int t = 0;
    for (int i = 0; i < n; i++) {
        auto u = p[i];
        auto v = p[(i + 1) % n];
        if (u.x < a.x && v.x >= a.x && side(a, Line(v, u))) {
            t ^= 1;
        }
        if (u.x >= a.x && v.x < a.x && side(a, Line(u, v))) {
            t ^= 1;
        }
    }

    return t == 1;
}

template <class T> tuple<int, Point<T>, Point<T>> segInt(const Line<T> &l1, const Line<T> &l2) {
    if (max(l1.a.x, l1.b.x) < min(l2.a.x, l2.b.x)) {
        return {0, Point<T>(), Point<T>()};
    }
    if (min(l1.a.x, l1.b.x) > max(l2.a.x, l2.b.x)) {
        return {0, Point<T>(), Point<T>()};
    }
    if (max(l1.a.y, l1.b.y) < min(l2.a.y, l2.b.y)) {
        return {0, Point<T>(), Point<T>()};
    }
    if (min(l1.a.y, l1.b.y) > max(l2.a.y, l2.b.y)) {
        return {0, Point<T>(), Point<T>()};
    }
    if (cross(l1.b - l1.a, l2.b - l2.a) == 0) {
        if (cross(l1.b - l1.a, l2.a - l1.a) != 0) {
            return {0, Point<T>(), Point<T>()};
        } else {
            auto xx1 = max(l1.a.x, l1.b.x);
            auto nx1 = min(l1.a.x, l1.b.x);
            auto yy1 = max(l1.a.y, l1.b.y);
            auto ny1 = min(l1.a.y, l1.b.y);
            auto xx2 = max(l2.a.x, l2.b.x);
            auto nx2 = min(l2.a.x, l2.b.x);
            auto yy2 = max(l2.a.y, l2.b.y);
            auto ny2 = min(l2.a.y, l2.b.y);
            Point<T> p1(max(nx1, nx2), max(ny1, ny2));
            Point<T> p2(min(xx1, xx2), min(yy1, yy2));
            if (!onSeg(p1, l1)) {
                swap(p1.y, p2.y);
            }
            if (p1 == p2) {
                return {3, p1, p2};
            } else {
                return {2, p1, p2};
            }
        }
    }
    auto cp1 = cross(l2.a - l1.a, l2.b - l1.a);
    auto cp2 = cross(l2.a - l1.b, l2.b - l1.b);
    auto cp3 = cross(l1.a - l2.a, l1.b - l2.a);
    auto cp4 = cross(l1.a - l2.b, l1.b - l2.b);

    if ((cp1 > 0 && cp2 > 0) || (cp1 < 0 && cp2 < 0) || (cp3 > 0 && cp4 > 0) || (cp3 < 0 && cp4 < 0)) {
        return {0, Point<T>(), Point<T>()};
    }

    Point p = inter(l1, l2);
    if (cp1 != 0 && cp2 != 0 && cp3 != 0 && cp4 != 0) {
        return {1, p, p};
    } else {
        return {3, p, p};
    }
}

template <class T> double disSS(const Line<T> &l1, const Line<T> &l2) {
    if (get<0>(segInt(l1, l2)) != 0) {
        return 0.0;
    }
    return min({disPS(l1.a, l2), disPS(l1.b, l2), disPS(l2.a, l1), disPS(l2.b, l1)});
}

template <class T> bool segIn(const Line<T> &l, const vector<Point<T>> &p) {
    int n = p.size();
    if (!inPoly(l.a, p)) {
        return false;
    }
    if (!inPoly(l.b, p)) {
        return false;
    }
    for (int i = 0; i < n; i++) {
        auto u = p[i];
        auto v = p[(i + 1) % n];
        auto w = p[(i + 2) % n];
        auto [t, p1, p2] = segInt(l, Line(u, v));

        if (t == 1) {
            return false;
        }
        if (t == 0) {
            continue;
        }
        if (t == 2) {
            if (onSeg(v, l) && v != l.a && v != l.b) {
                if (cross(v - u, w - v) > 0) {
                    return false;
                }
            }
        } else {
            if (p1 != u && p1 != v) {
                if (side(l.a, Line(v, u)) || side(l.b, Line(v, u))) {
                    return false;
                }
            } else if (p1 == v) {
                if (l.a == v) {
                    if (side(u, l)) {
                        if (side(w, l) && side(w, Line(u, v))) {
                            return false;
                        }
                    } else {
                        if (side(w, l) || side(w, Line(u, v))) {
                            return false;
                        }
                    }
                } else if (l.b == v) {
                    if (side(u, Line(l.b, l.a))) {
                        if (side(w, Line(l.b, l.a)) && side(w, Line(u, v))) {
                            return false;
                        }
                    } else {
                        if (side(w, Line(l.b, l.a)) || side(w, Line(u, v))) {
                            return false;
                        }
                    }
                } else {
                    if (side(u, l)) {
                        if (side(w, Line(l.b, l.a)) || side(w, Line(u, v))) {
                            return false;
                        }
                    } else {
                        if (side(w, l) || side(w, Line(u, v))) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

template <class T> vector<Point<T>> hp(vector<Line<T>> a) {
    sort(a.begin(), a.end(), [&](auto l1, auto l2) {
        auto d1 = l1.b - l1.a;
        auto d2 = l2.b - l2.a;

        if (sgn(d1) != sgn(d2)) {
            return sgn(d1) == 1;
        }

        return cross(d1, d2) > 0;
    });

    deque<Line<T>> ls;
    deque<Point<T>> ps;
    for (auto l : a) {
        if (ls.empty()) {
            ls.push_back(l);
            continue;
        }

        while (!ps.empty() && !side(ps.back(), l)) {
            ps.pop_back();
            ls.pop_back();
        }

        while (!ps.empty() && !side(ps[0], l)) {
            ps.pop_front();
            ls.pop_front();
        }

        if (cross(l.b - l.a, ls.back().b - ls.back().a) == 0) {
            if (dot(l.b - l.a, ls.back().b - ls.back().a) > 0) {
                if (!side(ls.back().a, l)) {
                    assert(ls.size() == 1);
                    ls[0] = l;
                }
                continue;
            }
            return {};
        }

        ps.push_back(inter(ls.back(), l));
        ls.push_back(l);
    }

    while (!ps.empty() && !side(ps.back(), ls[0])) {
        ps.pop_back();
        ls.pop_back();
    }
    if (ls.size() <= 2) {
        return {};
    }
    ps.push_back(inter(ls[0], ls.back()));

    return vector(ps.begin(), ps.end());
}

using Real = ld;
using P = Point<Real>;

constexpr Real eps = 0;
