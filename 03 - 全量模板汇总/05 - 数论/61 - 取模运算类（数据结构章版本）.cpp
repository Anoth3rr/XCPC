template <class T> constexpr T power(T n, int k) {
    T r = 1;
    for (; k; k >>= 1, n *= n)
        if (k & 1) r *= n;
    return r;
}

template <const int &MOD> struct MInt {
    int x;
    MInt(signed x = 0) : x(norm(x % MOD)) {}
    MInt(int x) : x(norm(x % MOD)) {}

    constexpr int norm(int x) const noexcept {
        if (x < 0) [[unlikely]] {
            x += MOD;
        }
        if (x >= MOD) [[unlikely]] {
            x -= MOD;
        }
        return x;
    }
    explicit operator int() const { return x; }
    constexpr int val() const { return x; }
    constexpr MInt operator-() const {
        MInt val = norm(MOD - x);
        return val;
    }
    constexpr MInt inv() const {
        assert(x != 0);
        return power(*this, MOD - 2);
    }
    friend constexpr auto &operator>>(istream &in, MInt &j) {
        int v;
        in >> v;
        j = MInt(v);
        return in;
    }
    friend constexpr auto &operator<<(ostream &o, const MInt &j) { return o << j.val(); }
    constexpr MInt &operator++() {
        x = norm(x + 1);
        return *this;
    }
    constexpr MInt &operator--() {
        x = norm(x - 1);
        return *this;
    }
    constexpr MInt operator++(signed) {
        MInt res = *this;
        ++*this;
        return res;
    }
    constexpr MInt operator--(signed) {
        MInt res = *this;
        --*this;
        return res;
    }
    constexpr MInt &operator+=(const MInt &i) {
        x = norm(x + i.x);
        return *this;
    }
    constexpr MInt &operator-=(const MInt &i) {
        x = norm(x - i.x);
        return *this;
    }
    constexpr MInt &operator*=(const MInt &i) {
        x = x * i.x % MOD;
        return *this;
    }
    constexpr MInt &operator/=(const MInt &i) { return *this *= i.inv(); }
    constexpr MInt &operator%=(const int &i) { return x %= i, *this; }
    friend constexpr MInt operator+(const MInt i, const MInt j) { return MInt(i) += j; }
    friend constexpr MInt operator-(const MInt i, const MInt j) { return MInt(i) -= j; }
    friend constexpr MInt operator*(const MInt i, const MInt j) { return MInt(i) *= j; }
    friend constexpr MInt operator/(const MInt i, const MInt j) { return MInt(i) /= j; }
    friend constexpr MInt operator%(const MInt i, const int j) { return MInt(i) %= j; }
    friend constexpr bool operator==(const MInt i, const MInt j) { return i.val() == j.val(); }
    friend constexpr bool operator!=(const MInt i, const MInt j) { return i.val() != j.val(); }
    friend constexpr bool operator<(const MInt i, const MInt j) { return i.val() < j.val(); }
    friend constexpr bool operator>(const MInt i, const MInt j) { return i.val() > j.val(); }
};

int MOD[] = {998244353, 1000000007};
using Z = MInt<MOD[1]>;
