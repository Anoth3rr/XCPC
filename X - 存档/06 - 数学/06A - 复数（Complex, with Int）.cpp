template <typename T> class Complex {
  public:
    T a, b;

    constexpr Complex(T r = 0, T i = 0) : a(r), b(i) {}

    constexpr Complex operator+(const Complex &rhs) const {
        return {a + rhs.a, b + rhs.b};
    }

    constexpr Complex operator-(const Complex &rhs) const {
        return {a - rhs.a, b - rhs.b};
    }

    constexpr Complex operator*(const Complex &rhs) const {
        return {a * rhs.a - b * rhs.b, a * rhs.b + b * rhs.a};
    }

    Complex operator/(const Complex &rhs) const {
        T den = rhs.a * rhs.a + rhs.b * rhs.b;
        assert(den != 0);
        return {(a * rhs.a + b * rhs.b) / den, (b * rhs.a - a * rhs.b) / den};
    }

    Complex inv() const {
        T den = a * a + b * b;
        assert(den != 0);
        return Complex(a / den, -b / den);
    }

    Complex &operator+=(const Complex &rhs) {
        *this = (*this) + rhs;
        return *this;
    }

    Complex &operator-=(const Complex &rhs) {
        *this = (*this) - rhs;
        return *this;
    }

    Complex &operator*=(const Complex &rhs) {
        *this = (*this) * rhs;
        return *this;
    }

    Complex &operator/=(const Complex &rhs) {
        *this = (*this) / rhs;
        return *this;
    }

    T norm() const {
        return a * a + b * b;
    }

    bool operator==(const Complex &rhs) const {
        return a == rhs.a && b == rhs.b;
    }

    bool operator!=(const Complex &rhs) const {
        return !(*this == rhs);
    }

    bool operator<(const Complex &rhs) const {
        return norm() < rhs.norm();
    }

    bool operator>(const Complex &rhs) const {
        return norm() > rhs.norm();
    }

    friend ostream &operator<<(ostream &os, const Complex &c) {
        os << "(" << c.a;
        if (c.b >= 0) os << "+";
        os << c.b << "i)";
        return os;
    }
};

template <class T> Complex<T> conj(const Complex<T> &z) {
    return Complex<T>(z.a, -z.b);
}
