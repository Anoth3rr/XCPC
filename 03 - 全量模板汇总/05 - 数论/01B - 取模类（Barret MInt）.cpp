constexpr int mod = 998244353;

template <class T> T power(T a, int b) {
    T r = T(1);
    for (; b; a *= a, b >>= 1)
        if (b & 1) r *= a;
    return r;
}

struct Barrett {
    using u128 = uint128_t;
    using u64 = uint64_t;
    int m;
    u128 B;
    Barrett(int m = 2) : m(m), B((u128(1) << 64) / m) {}
    friend int operator%(int a, const Barrett &mod) {
        u64 x = a < 0 ? -a : a;
        u64 q = mod.B * x >> 64;
        u64 r = x - q * mod.m;
        if (r >= mod.m) r -= mod.m;
        int ans = r;
        return a < 0 ? -ans : ans;
    }
    friend int operator%=(int &a, const Barrett &mod) {
        return a = a % mod;
    }
    friend int operator+(const int a, const Barrett &mod) {
        return a + mod.m;
    }
    friend int operator-(const int a, const Barrett &mod) {
        return a - mod.m;
    }
    friend int operator-(const Barrett &mod, const int a) {
        return mod.m - a;
    }
    friend int operator/(const Barrett &mod, const int a) {
        return mod.m / a;
    }
    friend int operator%(const Barrett &mod, const int a) {
        return mod.m % a;
    }
};

template <int P> struct MInt {
    int x;
    inline static constexpr Barrett Mod{P};

    constexpr MInt() : x{} {}
    constexpr MInt(int x) : x{norm(x)} {}

    constexpr int norm(int x) const {
        if (x < 0) {
            x = x + Mod;
        }
        if (x >= P) {
            x = x - Mod;
        }
        return x;
    }

    constexpr int val() const {
        return x;
    }
    explicit constexpr operator int() const {
        return x;
    }

    constexpr MInt operator-() const {
        MInt res;
        res.x = norm(P - x);
        return res;
    }
    constexpr MInt inv() const {
        assert(x != 0);
        return power(*this, P - 2);
    }
    constexpr MInt &operator*=(MInt rhs) & {
        x = 1LL * x * rhs.x % Mod;
        return *this;
    }
    constexpr MInt &operator+=(MInt rhs) & {
        x = norm(x + rhs.x);
        return *this;
    }
    constexpr MInt &operator-=(MInt rhs) & {
        x = norm(x - rhs.x);
        return *this;
    }
    constexpr MInt &operator/=(MInt rhs) & {
        return *this *= rhs.inv();
    }
    friend constexpr MInt operator*(MInt lhs, MInt rhs) {
        MInt res = lhs;
        res *= rhs;
        return res;
    }
    friend constexpr MInt operator+(MInt lhs, MInt rhs) {
        MInt res = lhs;
        res += rhs;
        return res;
    }
    friend constexpr MInt operator-(MInt lhs, MInt rhs) {
        MInt res = lhs;
        res -= rhs;
        return res;
    }
    friend constexpr MInt operator/(MInt lhs, MInt rhs) {
        MInt res = lhs;
        res /= rhs;
        return res;
    }
    friend constexpr istream &operator>>(istream &is, MInt &a) {
        int v;
        is >> v;
        a = MInt(v);
        return is;
    }
    friend constexpr ostream &operator<<(ostream &os, const MInt &a) {
        return os << a.val();
    }
    friend constexpr bool operator==(MInt lhs, MInt rhs) {
        return lhs.val() == rhs.val();
    }
    friend constexpr bool operator!=(MInt lhs, MInt rhs) {
        return lhs.val() != rhs.val();
    }
};

template <int V, int P> const MInt<P> IV = MInt<P>(V).inv();

using Z = MInt<mod>;