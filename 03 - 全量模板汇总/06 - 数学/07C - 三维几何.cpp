/*
用途：三维点、向量、直线与平面的基础运算。整数 T 可精确做点积/叉积/体积判断；距离返回 long double。
*/
using ld = long double;

template <class T> struct Point3 {
    T x{}, y{}, z{};
    Point3() = default;
    Point3(T x, T y, T z) : x(x), y(y), z(z) {}
    Point3& operator+=(const Point3& p) { x += p.x, y += p.y, z += p.z; return *this; }
    Point3& operator-=(const Point3& p) { x -= p.x, y -= p.y, z -= p.z; return *this; }
    Point3& operator*=(const T& k) { x *= k, y *= k, z *= k; return *this; }
    friend Point3 operator+(Point3 a, const Point3& b) { return a += b; }
    friend Point3 operator-(Point3 a, const Point3& b) { return a -= b; }
    friend Point3 operator*(Point3 a, const T& k) { return a *= k; }
    friend bool operator==(const Point3& a, const Point3& b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
};

template <class T> T dot(const Point3<T>& a, const Point3<T>& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
template <class T> Point3<T> cross(const Point3<T>& a, const Point3<T>& b) {
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}
template <class T> T square(const Point3<T>& a) { return dot(a, a); }
template <class T> T volume6(const Point3<T>& a, const Point3<T>& b, const Point3<T>& c, const Point3<T>& d) {
    return dot(b - a, cross(c - a, d - a));
}
template <class T> ld disPL(const Point3<T>& p, const Point3<T>& a, const Point3<T>& b) {
    return sqrtl(static_cast<ld>(square(cross(p - a, b - a))) /
                 static_cast<ld>(square(b - a)));
}
template <class T> ld disPP(const Point3<T>& p, const Point3<T>& a, const Point3<T>& n) {
    return fabsl(static_cast<ld>(dot(p - a, n))) /
           sqrtl(static_cast<ld>(square(n)));
}

// 直线 p+t*d 与平面 (x-a)·n=0 的交点；平行时返回 nullopt。
template <class T>
optional<Point3<ld>> interLP(const Point3<T>& p, const Point3<T>& d,
                                             const Point3<T>& a, const Point3<T>& n) {
    T z = dot(d, n);
    if (z == 0) return nullopt;
    ld t = static_cast<ld>(dot(a - p, n)) / static_cast<ld>(z);
    return Point3<ld>(p.x + t * d.x, p.y + t * d.y, p.z + t * d.z);
}
