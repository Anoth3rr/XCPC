/*
用途：求 h(i)=[x^N]f(x)g(x)^i (0<=i<=n) 的极致优化版。
依赖：08I - 快速数论变换（NTT，极致优化）。接口：PPFast::ask(f, g, N, n)。
限制：固定模数 998244353；n < mod，N+1 <= 2^22，输入会截断到 N 次。
区别：08O 为短小通用基线；本模板将二元消元与 NTT 排布融合，适合 N、n 都大时。
性能：大数组存 Montgomery 32 位 Z，循环下标用 i32；改回宏展开后的 int 会明显增大内存。
*/

struct PPFast {
    using T = NTT<998244353, 3>;
    using Z = T::Z;

    static vector<Z> mul(vector<Z> a, vector<Z> b) {
        if (a.empty() || b.empty()) return {};
        i32 m = a.size() + b.size() - 1;
        if (min(a.size(), b.size()) <= 32) {
            vector<Z> c(m);
            for (i32 i = 0; i < a.size(); ++i)
                for (i32 j = 0; j < b.size(); ++j) c[i + j] += a[i] * b[j];
            return c;
        }
        i32 n = 1;
        while (n < m) n <<= 1;
        a.resize(n), b.resize(n);
        T::ntt(a, false);
        T::ntt(b, false);
        for (i32 i = 0; i < n; ++i) a[i] *= b[i];
        T::ntt(a, true);
        a.resize(m);
        return a;
    }

    static vector<Z> pp0(vector<Z> wt, vector<Z> h, i32 m) {
        i32 len = 1;
        while (len < h.size()) len <<= 1;
        assert(len <= (1 << 22));
        h.resize(len), wt.resize(len);
        for (Z& x : h) x = Z(0) - x;
        reverse(wt.begin(), wt.end());
        vector<Z> p = wt, q = h;
        p.resize(4 * len), q.resize(4 * len);

        vector<Z> w(len);
        if (len > 1) {
            T::init();
            vector<i32> rv(len);
            i32 lg = __builtin_ctz(len);
            for (i32 i = 0; i < len; ++i) rv[i] = (rv[i >> 1] >> 1) | ((i & 1) << (lg - 1));
            Z dw = T::rt[23].inv().pow((1 << 23) / (2 * len)), z = 1;
            for (i32 i : rv) w[i] = z, z *= dw;
        } else w[0] = 1;

        for (i32 k = 1, cur = len; cur > 1; cur >>= 1, k <<= 1) {
            vector<Z> fy(k), hx(2 * cur), f(2 * cur), g(2 * cur), x(cur), y(cur);
            auto dy = [&](vector<Z>& a, i32 l, i32 r) {
                Z z = w[k >> 1].inv();
                for (i32 i = l; i < r; ++i) {
                    for (i32 j = 0; j < k; ++j) fy[j] = a[2 * cur * j + i];
                    T::ntt(fy, true);
                    Z v = z;
                    for (i32 j = 1; j < k; ++j, v *= z) fy[j] *= v;
                    T::ntt(fy, false);
                    for (i32 j = 0; j < k; ++j) a[2 * cur * (k + j) + i] = fy[j];
                }
            };
            auto fx = [&](vector<Z>& a, i32 l, i32 r) {
                for (i32 j = l; j < r; ++j) {
                    for (i32 i = 0; i < 2 * cur; ++i) hx[i] = a[2 * cur * j + i];
                    T::ntt(hx, false);
                    for (i32 i = 0; i < 2 * cur; ++i) a[2 * cur * j + i] = hx[i];
                }
            };

            if (cur <= k) {
                dy(p, 0, cur), dy(q, 1, cur);
                fx(p, 0, 2 * k), fx(q, 0, 2 * k);
            } else {
                fx(p, 0, k), fx(q, 0, k);
                dy(p, 0, 2 * cur), dy(q, 0, 2 * cur);
            }
            for (i32 i = 0; i < 2 * cur * k; ++i) q[i] += Z(1);
            for (i32 i = 2 * cur * k; i < 4 * cur * k; ++i) q[i] -= Z(1);

            for (i32 j = 0; j < 2 * k; ++j) {
                for (i32 i = 0; i < 2 * cur; ++i) f[i] = p[2 * cur * j + i], g[i] = q[2 * cur * j + i];
                for (i32 i = 0; i < cur; ++i) {
                    x[i] = w[i] * (f[2 * i] * g[2 * i + 1] - f[2 * i + 1] * g[2 * i]);
                    y[i] = g[2 * i] * g[2 * i + 1];
                }
                T::ntt(x, true), T::ntt(y, true);
                fill(x.begin() + cur / 2, x.end(), Z(0));
                fill(y.begin() + cur / 2, y.end(), Z(0));
                for (i32 i = 0; i < cur; ++i) p[cur * j + i] = x[i], q[cur * j + i] = y[i];
            }
            fill(p.begin() + 2 * cur * k, p.end(), Z(0));
            fill(q.begin() + 2 * cur * k, q.end(), Z(0));
            for (i32 j = 0; j < 4 * k; ++j) q[cur * j] = Z(0);
        }

        for (i32 i = 0; i < len; ++i) p[i] = p[2 * i];
        p.resize(len);
        Z z = Z(len).inv();
        for (Z& x : p) x *= z;
        T::ntt(p, true);
        reverse(p.begin(), p.end());
        p.resize(m + 1);
        return p;
    }

    static vector<int> ask(vector<int> f, vector<int> g, int N, int n) {
        assert(N >= 0 && n >= 0 && n < 998244353);
        f.resize(N + 1), g.resize(N + 1);
        vector<Z> a(N + 1), b(N + 1), fc(n + 1), ifc(n + 1);
        for (i32 i = 0; i <= N; ++i) a[i] = Z(f[i]), b[i] = Z(g[i]);
        fc[0] = 1;
        for (i32 i = 1; i <= n; ++i) fc[i] = fc[i - 1] * Z(i);
        ifc[n] = fc[n].inv();
        for (i32 i = n; i; --i) ifc[i - 1] = ifc[i] * Z(i);

        auto pp = [&](auto&& self, vector<Z> wt, vector<Z> h, i32 m) -> vector<Z> {
            if (wt.size() < h.size()) wt.resize(h.size());
            else if (h.size() < wt.size()) h.resize(wt.size());
            if (h.empty()) return vector<Z>(m + 1);
            if (h[0].val()) {
                Z c = h[0];
                h[0] = 0;
                vector<Z> x = self(self, wt, h, m);
                for (i32 i = 0; i <= m; ++i) x[i] *= ifc[i];
                vector<Z> y(m + 1);
                for (i32 i = 0; i <= m; ++i) y[i] = ifc[i] * c.pow(i);
                x = mul(x, y);
                x.resize(m + 1);
                for (i32 i = 0; i <= m; ++i) x[i] *= fc[i];
                return x;
            }
            if (h.size() == 1) {
                vector<Z> x(m + 1);
                x[0] = wt[0];
                return x;
            }
            return pp0(wt, h, m);
        };

        vector<Z> wt(N + 1);
        for (i32 i = 0; i <= N; ++i) wt[i] = a[N - i];
        vector<Z> z = pp(pp, wt, b, n);
        vector<int> r(n + 1);
        for (i32 i = 0; i <= n; ++i) r[i] = z[i].val();
        return r;
    }
};
