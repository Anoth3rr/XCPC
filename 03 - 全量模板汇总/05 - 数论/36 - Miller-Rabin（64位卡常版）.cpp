using u64 = uint64_t;
using u128 = __uint128_t;

struct Montgomery {
    u64 m, m2, im, l1, l2;
    Montgomery() {}
    Montgomery(u64 m) : m(m) {
        l1 = -(u64)m % m, l2 = -(u128)m % m;
        m2 = m << 1, im = m;
        for (int i = 0; i < 5; i++) {
            im *= 2 - m * im;
        }
    }
    inline u64 operator()(i64 a, i64 b) const {
        u128 c = (u128)a * b;
        return u64(c >> 64) + m - u64((u64)c * im * (u128)m >> 64);
    }
    inline u64 reduce(u64 a) const {
        a = m - u64(a * im * (u128)m >> 64);
        return a >= m ? a - m : a;
    }
    inline u64 trans(i64 a) const {
        return (*this)(a, l2);
    }

    inline u64 mul(i64 a, i64 b) const {
        u64 r = (*this)(trans(a), trans(b));
        return reduce(r);
    }
    u64 pow(u64 a, u64 n) {
        u64 r = l1;
        a = trans(a);
        for (; n; n >>= 1, a = (*this)(a, a)) {
            if (n & 1) r = (*this)(r, a);
        }
        return reduce(r);
    }
};

bool isprime(i64 n) {
    if (n < 2 || n % 6 % 4 != 1) {
        return (n | 1) == 3;
    }
    u64 s = __builtin_ctzll(n - 1), d = n >> s;
    Montgomery M(n);
    for (i64 a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) {
        u64 p = M.pow(a, d), i = s;
        while (p != 1 && p != n - 1 && a % n && i--) {
            p = M.mul(p, p);
        }
        if (p != n - 1 && i != s) return false;
    }
    return true;
}
