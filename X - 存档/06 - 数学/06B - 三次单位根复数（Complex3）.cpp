// x^3 = 1, x != 1
template <class T> struct Complex3 {
    T a, b;
    Complex3(T a = T(), T b = T()) : a(a), b(b) {}

    Complex3 operator+(const Complex3 &rhs) const {
        return {a + rhs.a, b + rhs.b};
    }

    Complex3 operator-(const Complex3 &rhs) const {
        return {a - rhs.a, b - rhs.b};
    }

    Complex3 operator*(const Complex3 &rhs) const {
        T ac = a * rhs.a;
        T bd = b * rhs.b;
        T ad = a * rhs.b;
        T bc = b * rhs.a;

        return {ac - bd, ad + bc - bd};
    }

    Complex3 operator/(const Complex3 &rhs) const {
        return (*this) * rhs.inv();
    }

    Complex3 inv() const {
        T z = a * a - a * b + b * b;
        assert(z != 0);
        return Complex3<T>((a - b) / z, -b / z);
    }

    Complex3 &operator+=(const Complex3 &rhs) {
        *this = (*this) + rhs;
        return *this;
    }

    Complex3 &operator-=(const Complex3 &rhs) {
        *this = (*this) - rhs;
        return *this;
    }

    Complex3 &operator*=(const Complex3 &rhs) {
        *this = (*this) * rhs;
        return *this;
    }

    Complex3 &operator/=(const Complex3 &rhs) {
        *this = (*this) / rhs;
        return *this;
    }
    bool ask0() const {
        return a == 0 && b == 0;
    }
};
