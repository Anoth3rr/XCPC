constexpr int mod = 998244353;

template <class T> T mypow(T a, int n) {
    T r = T(1);
    for (; n; a *= a, n >>= 1)
        if (n & 1) r *= a;
    return r;
}

template <int P> struct MInt {
    int x = 0;
    constexpr MInt() : x{} {}
    constexpr MInt(int x) : x{norm(x % getMod())} {}

    inline static int md = P ? P : mod;
    static int getMod() {
        return P ? P : md;
    }

    static void setMod(int v) {
        md = v;
    }

    static int norm(int v) {
        v %= getMod();
        if (v < 0) v += getMod();
        return v;
    }

    int val() const {
        return x;
    }

    explicit operator int() const {
        return x;
    }

    MInt pow(int n) const {
        return mypow(*this, n);
    }

    MInt inv() const {
        assert(x != 0);
        return pow(getMod() - 2);
    }

    MInt operator-() const {
        return MInt(x ? getMod() - x : 0);
    }

    MInt &operator+=(MInt a) {
        int p = getMod();
        if (x >= p - a.x)
            x -= p - a.x;
        else
            x += a.x;
        return *this;
    }

    MInt &operator-=(MInt a) {
        x -= a.x;
        if (x < 0) x += getMod();
        return *this;
    }

    MInt &operator*=(MInt a) {
        x = static_cast<int>(static_cast<int>(x) * a.x % getMod());
        return *this;
    }

    MInt &operator/=(MInt a) {
        return *this *= a.inv();
    }

    MInt &operator++() {
        return *this += 1;
    }

    MInt operator++(int) {
        MInt a = *this;
        ++*this;
        return a;
    }

    MInt &operator--() {
        return *this -= 1;
    }

    MInt operator--(int) {
        MInt a = *this;
        --*this;
        return a;
    }

    friend MInt operator+(MInt a, MInt b) {
        return a += b;
    }

    friend MInt operator-(MInt a, MInt b) {
        return a -= b;
    }

    friend MInt operator*(MInt a, MInt b) {
        return a *= b;
    }

    friend MInt operator/(MInt a, MInt b) {
        return a /= b;
    }

    friend bool operator==(MInt a, MInt b) {
        return a.x == b.x;
    }

    friend bool operator!=(MInt a, MInt b) {
        return a.x != b.x;
    }

    friend istream &operator>>(istream &is, MInt &a) {
        int x;
        is >> x;
        a = MInt(x);
        return is;
    }

    friend ostream &operator<<(ostream &os, const MInt &a) {
        return os << a.x;
    }
};

template <int V, int P> const MInt<P> IV = MInt<P>(V).inv();

using Z = MInt<mod>;