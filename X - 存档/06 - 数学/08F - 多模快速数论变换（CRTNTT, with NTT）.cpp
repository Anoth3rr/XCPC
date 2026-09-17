/*
用途：用三模 NTT + CRT 做任意 32 位模数卷积。
依赖：08 - 快速数论变换（NTT）。
用法：原始整数写 mul(vector<int>, vector<int>, p)；
      自动取模写 mul(vector<Z>, vector<Z>)，Z 为 MInt<P>。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/

using i128 = __int128_t;

struct CRTNTT {
    static constexpr int p1 = 998244353;
    static constexpr int p2 = 1004535809;
    static constexpr int p3 = 104857601;

    NTT<p1, 3> a;
    NTT<p2, 3> b;
    NTT<p3, 3> c;

    static int mul(int a, int b, int p) {
        assert(p > 0);
        a %= p;
        b %= p;
#ifdef __SIZEOF_INT128__
        return i128(a) * b % p;
#else
        int r = 0;
        while (b) {
            if (b & 1) r = r >= p - a ? r - (p - a) : r + a;
            a = a >= p - a ? a - (p - a) : a + a;
            b >>= 1;
        }
        return r;
#endif
    }

    static int crt(int a, int b, int c, int p) {
        assert(p > 0);
        static constexpr int m1 = p1, m2 = p2, m3 = p3;
        static constexpr int i1 = 669690699;
        static constexpr int i2 = 99747662;
        int x = a;
        int t = (b - x) * i1 % m2;
        if (t < 0) t += m2;
        x += t * m1;
        t = (c - x % m3) * i2 % m3;
        if (t < 0) t += m3;
        int y = mul(t, mul(m1 % p, m2 % p, p), p);
        int z = x % p;
        return z >= p - y ? z - (p - y) : z + y;
    }

    vector<int> mul(const vector<int>& x, const vector<int>& y, int p) {
        if (x.empty() || y.empty()) return {};
        auto u = a.mul(x, y);
        auto v = b.mul(x, y);
        auto w = c.mul(x, y);
        vector<int> r(u.size());
        for (int i = 0; i < u.size(); ++i) r[i] = crt(u[i], v[i], w[i], p);
        return r;
    }

    template <class Z> vector<Z> mul(const vector<Z>& x, const vector<Z>& y) {
        vector<int> a(x.size()), b(y.size());
        for (int i = 0; i < x.size(); ++i) a[i] = static_cast<int>(x[i]);
        for (int i = 0; i < y.size(); ++i) b[i] = static_cast<int>(y[i]);
        vector<int> v = mul(a, b, Z::askMod());
        vector<Z> r(v.size());
        for (int i = 0; i < v.size(); ++i) r[i] = Z(v[i]);
        return r;
    }
};
