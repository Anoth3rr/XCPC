struct Barrett {
    uint32_t m;
    uint64_t im;
    Barrett(uint32_t m = 1) : m(m), im(uint64_t(-1) / m + 1) {}
    uint32_t mul(uint32_t a, uint32_t b) const {
        uint64_t z = uint64_t(a) * b;
        uint64_t q = __uint128_t(z) * im >> 64;
        uint32_t y = z - q * m;
        return y < m ? y : uint32_t(y + m);
    }
};

template <int id = 0> struct MInt {
    inline static Barrett B;
    uint32_t x;
    MInt(i64 v = 0) : x(norm(v)) {}
    static void setMod(int m) {
        assert(1 < m && m < (1LL << 31));
        B = Barrett(m);
    }
    static int getMod() {
        return B.m;
    }
    static uint32_t norm(int v) {
        v %= getMod();
        return v < 0 ? v + getMod() : v;
    }
    int val() const {
        return x;
    }
    MInt pow(int k) const {
        assert(k >= 0);
        MInt a = *this, r = 1;
        for (; k; k >>= 1, a *= a)
            if (k & 1) r *= a;
        return r;
    }
    MInt inv() const {
        assert(x);
        return pow(getMod() - 2);
    }
    MInt operator-() const { return x ? MInt(getMod() - x) : MInt(0); }
    MInt &operator+=(MInt v) {
        x += v.x;
        if (x >= B.m) x -= B.m;
        return *this;
    }
    MInt &operator-=(MInt v) {
        x = x >= v.x ? x - v.x : x + B.m - v.x;
        return *this;
    }
    MInt &operator*=(MInt v) {
        x = B.mul(x, v.x);
        return *this;
    }
    MInt &operator/=(MInt v) { return *this *= v.inv(); }
    friend MInt operator+(MInt a, MInt b) { return a += b; }
    friend MInt operator-(MInt a, MInt b) { return a -= b; }
    friend MInt operator*(MInt a, MInt b) { return a *= b; }
    friend MInt operator/(MInt a, MInt b) { return a /= b; }
    friend bool operator==(MInt a, MInt b) { return a.x == b.x; }
    friend bool operator!=(MInt a, MInt b) { return a.x != b.x; }
    friend istream &operator>>(istream &in, MInt &a) {
        i64 v;
        in >> v;
        a = v;
        return in;
    }
    friend ostream &operator<<(ostream &out, MInt a) { return out << a.x; }
};
