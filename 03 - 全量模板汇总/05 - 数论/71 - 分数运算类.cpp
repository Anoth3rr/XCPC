template<class T = i64> struct Frac {
    T x, y;
    Frac(T x = 0, T y = 1) : x(x), y(y) { reduce(); }
    static T ab(T v) { return v < 0 ? -v : v; }
    void reduce() {
        assert(y != T(0));
        if (y < 0) x = -x, y = -y;
        if (x == T(0)) { y = 1; return; }
        T g = gcd(ab(x), y);
        x = x / g, y = y / g;
    }
    Frac norm() const { return Frac(x, y); }
    double val() const { return 1. * x / y; }
    Frac &operator+=(const Frac &v) {
        T g = gcd(y, v.y);
        x = x * (v.y / g) + v.x * (y / g), y = y / g * v.y;
        reduce();
        return *this;
    }
    Frac &operator-=(const Frac &v) { return *this += -v; }
    Frac &operator*=(const Frac &v) {
        T g1 = gcd(ab(x), v.y), g2 = gcd(ab(v.x), y);
        x = x / g1 * (v.x / g2), y = y / g2 * (v.y / g1);
        return *this;
    }
    Frac &operator/=(const Frac &v) {
        assert(v.x != T(0));
        T g1 = gcd(ab(x), ab(v.x)), g2 = gcd(y, v.y), vx = v.x / g1;
        x = x / g1 * (v.y / g2), y = y / g2 * vx;
        if (y < 0) x = -x, y = -y;
        return *this;
    }
    friend Frac operator+(Frac a, const Frac &b) { return a += b; }
    friend Frac operator-(Frac a, const Frac &b) { return a -= b; }
    friend Frac operator*(Frac a, const Frac &b) { return a *= b; }
    friend Frac operator/(Frac a, const Frac &b) { return a /= b; }
    friend Frac operator-(Frac a) { return Frac(-a.x, a.y); }
    friend bool operator<(const Frac &a, const Frac &b) {
        using W = conditional_t<is_integral_v<T>, i128, T>;
        return W(a.x) * W(b.y) < W(b.x) * W(a.y);
    }
    friend bool operator>(const Frac &a, const Frac &b) { return b < a; }
    friend bool operator<=(const Frac &a, const Frac &b) { return !(b < a); }
    friend bool operator>=(const Frac &a, const Frac &b) { return !(a < b); }
    friend bool operator==(const Frac &a, const Frac &b) { return a.x == b.x && a.y == b.y; }
    friend bool operator!=(const Frac &a, const Frac &b) { return !(a == b); }
    friend ostream &operator<<(ostream &out, const Frac &a) {
        return a.y == T(1) ? out << a.x : out << a.x << '/' << a.y;
    }
};
