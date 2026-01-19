template <typename Int = ll>
struct Frac {
    Int x, y;

    Frac() : Frac(0, 1) {}
    Frac(Int x_, Int y_) : x(x_), y(y_) {
        Int g = gcd(x, y);
        x /= g, y /= g;
        if (y < 0)
            x = -x, y = -y;
    }
    constexpr double val() const {
        return 1.0 * x / y;
    }
    friend constexpr auto &operator<<(ostream &o, const Frac &j) {
        Int g = gcd(j.x, j.y);
        if (j.y == p) {
            return o << j.x / g;
        } else {
            return o << j.x / g << "/" << j.y / g;
        }
    }
    constexpr Frac &operator/=(const Frac &i) {
        return Frac(x * i.y, y * i.x);
    }
    constexpr Frac &operator+=(const Frac &i) {
        return Frac(x * i.y + y * i.x, y * i.y);
    }
    constexpr Frac &operator-=(const Frac &i) {
        return Frac(x * i.y - y * i.x, y * i.y);
    }
    constexpr Frac &operator*=(const Frac &i) {
        return Frac(x * i.x, y * i.y);
    }
    friend constexpr Frac operator+(const Frac i, const Frac j) { return i += j; }
    friend constexpr Frac operator-(const Frac i, const Frac j) { return i -= j; }
    friend constexpr Frac operator*(const Frac i, const Frac j) { return i *= j; }
    friend constexpr Frac operator/(const Frac i, const Frac j) { return i /= j; }
    friend constexpr Frac operator-(const Frac i) { return Frac(-i.x, i.y); }
    friend constexpr bool operator<(const Frac i, const Frac j) { return i.x * j.y < i.y * j.x; }
    friend constexpr bool operator>(const Frac i, const Frac j) { return i.x * j.y > i.y * j.x; }
    friend constexpr bool operator==(const Frac i, const Frac j) { return i.x * j.y == i.y * j.x; }
    friend constexpr bool operator!=(const Frac i, const Frac j) { return i.x * j.y != i.y * j.x; }
};