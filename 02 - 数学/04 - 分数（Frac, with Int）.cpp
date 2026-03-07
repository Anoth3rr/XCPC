template <class Int>
struct Frac {
    Int x, y;
    Frac() : x(0), y(1) {}
    Frac(Int a, Int b) : x(a), y(b) {
        assert(y != 0);
        Int g = gcd(x, y);
        assert(g != 0);
        x /= g, y /= g;
        if (y < 0) x = -x, y = -y;
    }

    constexpr double val() const { return 1.0 * x / y; }

    friend ostream &operator<<(ostream &o, const Frac &b) {
        Int g = gcd(b.x, b.y);
        if (b.y == g) {
            return o << b.x / g;
        } else {
            return o << b.x / g << "/" << b.y / g;
        }
    }
    Frac &operator+=(const Frac o) {
        *this = Frac(x * o.y + y * o.x, y * o.y);
        return *this;
    }
    Frac &operator-=(const Frac o) {
        *this = Frac(x * o.y - y * o.x, y * o.y);
        return *this;
    }
    Frac &operator*=(const Frac o) {
        *this = Frac(x * o.x, y * o.y);
        return *this;
    }
    Frac &operator/=(const Frac o) {
        assert(o.x != 0);
        *this = Frac(x * o.y, y * o.x);
        return *this;
    }

    constexpr Frac operator-() const {
        *this = Frac(-x, y);
        return *this;
    }
    friend Frac operator+(Frac a, const Frac &b) { return a += b; }
    friend Frac operator-(Frac a, const Frac &b) { return a -= b; }
    friend Frac operator*(Frac a, const Frac &b) { return a *= b; }
    friend Frac operator/(Frac a, const Frac &b) { return a /= b; }
    friend bool operator<(const Frac &a, const Frac &b) { return a.x * b.y < a.y * b.x; }
    friend bool operator>(const Frac &a, const Frac &b) { return a.x * b.y > a.y * b.x; }
    friend bool operator==(const Frac &a, const Frac &b) { return a.x * b.y == a.y * b.x; }
    friend bool operator!=(const Frac &a, const Frac &b) { return a.x * b.y != a.y * b.x; }
};