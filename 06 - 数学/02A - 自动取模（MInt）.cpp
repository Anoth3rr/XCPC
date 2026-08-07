/*
用途：自动取模整数。直接写 MInt<P>，P=0 时先 MInt<0>::setMod(p)。
接口：pow(n)、inv()、四则运算、输入输出；默认 Z 为模 998244353。
约定：inv()/除法要求模数为质数；动态模数需满足 1 < p < 2^31。
性能：内部值固定为 i32，故在 #define int long long 下也不会把大数组翻倍。
*/

using i32 = int32_t;

constexpr i32 mod = 998244353;

template <class T> T mypow(T a, int n) {
    assert(n >= 0);
    T r = T(1);
    while (n) {
        if (n & 1) r *= a;
        a *= a;
        n >>= 1;
    }
    return r;
}

template <i32 P> struct MInt {
    static_assert(P >= 0);

    i32 x = 0;
    inline static i32 md = P ? P : mod;

    MInt() = default;
    MInt(int v) : x(norm(v)) {}

    static i32 askMod() {
        return P ? P : md;
    }

    static void setMod(i32 v) {
        static_assert(P == 0, "Only MInt<0> supports a dynamic modulus.");
        assert(v > 1);
        md = v;
    }

    static i32 norm(int v) {
        v %= askMod();
        if (v < 0) v += askMod();
        return static_cast<i32>(v);
    }

    i32 val() const {
        return x;
    }

    explicit operator i32() const {
        return x;
    }

    bool ask0() const {
        return x == 0;
    }

    MInt pow(int n) const {
        return mypow(*this, n);
    }

    MInt inv() const {
        assert(x);
        return pow(askMod() - 2);
    }

    MInt operator-() const {
        return MInt(x ? askMod() - x : 0);
    }

    MInt& operator+=(MInt a) {
        i32 p = askMod();
        if (x >= p - a.x) x -= p - a.x;
        else x += a.x;
        return *this;
    }

    MInt& operator-=(MInt a) {
        x -= a.x;
        if (x < 0) x += askMod();
        return *this;
    }

    MInt& operator*=(MInt a) {
        x = static_cast<i32>(static_cast<int>(x) * a.x % askMod());
        return *this;
    }

    MInt& operator/=(MInt a) {
        return *this *= a.inv();
    }

    MInt& operator++() {
        return *this += 1;
    }

    MInt operator++(i32) {
        MInt a = *this;
        ++*this;
        return a;
    }

    MInt& operator--() {
        return *this -= 1;
    }

    MInt operator--(i32) {
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

    friend istream& operator>>(istream& is, MInt& a) {
        int x;
        is >> x;
        a = MInt(x);
        return is;
    }

    friend ostream& operator<<(ostream& os, const MInt& a) {
        return os << a.x;
    }
};

template <i32 V, i32 P> const MInt<P> IV = MInt<P>(V).inv();

using Z = MInt<mod>;
