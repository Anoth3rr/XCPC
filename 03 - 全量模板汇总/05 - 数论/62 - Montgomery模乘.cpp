using u64 = uint64_t;
using u128 = __uint128_t;

struct Montgomery {
    u64 m, m2, im, l1, l2;
    Montgomery() {}
    Montgomery(u64 m) : m(m) {
        l1 = -(u64)m % m, l2 = -(u128)m % m;
        m2 = m << 1, im = m;
        for (int i = 0; i < 5; i++) im *= 2 - m * im;
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
    
    inline u64 add(i64 a, i64 b) const {
        u64 c = trans(a) + trans(b);
        if (c >= m2) c -= m2;
        return reduce(c);
    }
    inline u64 sub(i64 a, i64 b) const {
        u64 c = trans(a) - trans(b);
        if (c >= m2) c += m2;
        return reduce(c);
    }
    inline u64 mul(i64 a, i64 b) const {
        return reduce((*this)(trans(a), trans(b)));
    }
    inline u64 div(i64 a, i64 b) const {
        a = trans(a), b = trans(b);
        u64 n = m - 2, inv = l1;
        for (; n; n >>= 1, b = (*this)(b, b))
            if (n & 1) inv = (*this)(inv, b);
        return reduce((*this)(a, inv));
    }
    u64 pow(u64 a, u64 n) {
        u64 r = l1;
        a = trans(a);
        for (; n; n >>= 1, a = (*this)(a, a))
            if (n & 1) r = (*this)(r, a);
        return reduce(r);
    }
};
