/*
用途：任意 32 位模数卷积（系数按 15 位拆分）。
依赖：06A - 复数（Complex）。
用法：原始整数写 mul(vector<int>, vector<int>, p)；
      自动取模写 mul(vector<Z>, vector<Z>)，Z 为 MInt<P>。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/

struct FFT {
    using C = Complex<double>;

    static constexpr double pi = 3.141592653589793238462643383279502884;

    static void fft(vector<C>& a, bool iv) {
        int n = a.size();
        assert(n && !(n & (n - 1)));
        static vector<int> rv;
        static vector<C> rt{{0, 0}, {1, 0}};
        if (rv.size() != n) {
            int k = __builtin_ctz(static_cast<unsigned>(n));
            rv.assign(n, 0);
            if (n > 1)
                for (int i = 0; i < n; ++i) rv[i] = (rv[i >> 1] >> 1) | ((i & 1) << (k - 1));
        }
        if (rt.size() < n) {
            int k = __builtin_ctz(static_cast<unsigned>(rt.size()));
            rt.resize(n);
            while ((1LL << k) < n) {
                double ag = 2 * pi / (1LL << (k + 1));
                for (int i = 1LL << (k - 1); i < (1LL << k); ++i) {
                    rt[i << 1] = rt[i];
                    double x = ag * (2 * i + 1 - (1LL << k));
                    rt[i << 1 | 1] = C(cos(x), sin(x));
                }
                ++k;
            }
        }
        for (int i = 0; i < n; ++i)
            if (i < rv[i]) swap(a[i], a[rv[i]]);
        for (int len = 1; len < n; len <<= 1) {
            for (int i = 0; i < n; i += len << 1) {
                for (int j = 0; j < len; ++j) {
                    C x = a[i + j], y = a[i + j + len] * rt[len + j];
                    a[i + j] = x + y;
                    a[i + j + len] = x - y;
                }
            }
        }
        if (iv) {
            reverse(a.begin() + 1, a.end());
            for (C& x : a) x /= n;
        }
    }

    static vector<int> mul(const vector<int>& a, const vector<int>& b, int p) {
        if (a.empty() || b.empty()) return {};
        assert(p > 0);
        int m = a.size() + b.size() - 1, n = 1;
        while (n < m) n <<= 1;
        vector<C> x(n), y(n);
        for (int i = 0; i < a.size(); ++i) x[i] = C(a[i] & 32767, a[i] >> 15);
        for (int i = 0; i < b.size(); ++i) y[i] = C(b[i] & 32767, b[i] >> 15);
        fft(x, false);
        fft(y, false);
        vector<C> u(n), v(n);
        for (int i = 0; i < n; ++i) {
            int j = (n - i) & (n - 1);
            C a1 = (x[i] + conj(x[j])) * C(0.5, 0);
            C a2 = (x[i] - conj(x[j])) * C(0, -0.5);
            C b1 = (y[i] + conj(y[j])) * C(0.5, 0);
            C b2 = (y[i] - conj(y[j])) * C(0, -0.5);
            u[i] = a1 * b1 + a2 * b2 * C(0, 1);
            v[i] = a1 * b2 + a2 * b1;
        }
        fft(u, true);
        fft(v, true);
        vector<int> r(m);
        for (int i = 0; i < m; ++i) {
            int x0 = llround(u[i].a) % p;
            int y0 = llround(v[i].a) % p;
            int z0 = llround(u[i].b) % p;
            r[i] = (x0 + (y0 << 15) + (z0 << 30)) % p;
        }
        return r;
    }

    template <class Z> static vector<Z> mul(const vector<Z>& x, const vector<Z>& y) {
        vector<int> a(x.size()), b(y.size());
        for (int i = 0; i < x.size(); ++i) a[i] = static_cast<int>(x[i]);
        for (int i = 0; i < y.size(); ++i) b[i] = static_cast<int>(y[i]);
        vector<int> v = mul(a, b, Z::askMod());
        vector<Z> r(v.size());
        for (int i = 0; i < v.size(); ++i) r[i] = Z(v[i]);
        return r;
    }
};
