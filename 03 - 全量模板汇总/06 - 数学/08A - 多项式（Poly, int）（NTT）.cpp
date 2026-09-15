/*
用途：高性能整数系数多项式（乘法、求逆、ln/exp、求值、插值）。
依赖：08 - 快速数论变换（NTT）。
用法：using P = Poly<998244353, 3>；系数容器为 vector<int>。
约定：这里刻意不用 MInt，系数/下标缓存使用 int。
自动取模版本使用 08B / 08C 的 Poly<Z, 3>。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/

template <int P, int G = 3> struct Poly {

    static int pw(int a, int n) {
        return NTT<P, G>::pow(a, n);
    }

    static void ntt(vector<int>& a, bool iv) {
        NTT<P, G>::ntt(a, iv);
    }

    static vector<int> add(vector<int> a, vector<int> b) {
        int n = max(a.size(), b.size());
        a.resize(n);
        b.resize(n);
        for (int i = 0; i < n; ++i) {
            a[i] += b[i];
            if (a[i] >= P) a[i] -= P;
        }
        return a;
    }

    static vector<int> sub(vector<int> a, vector<int> b) {
        int n = max(a.size(), b.size());
        a.resize(n);
        b.resize(n);
        for (int i = 0; i < n; ++i) {
            a[i] -= b[i];
            if (a[i] < 0) a[i] += P;
        }
        return a;
    }

    static vector<int> mul(vector<int> a, vector<int> b) {
        return NTT<P, G>::mul(move(a), move(b));
    }

    static vector<int> inv(const vector<int>& a, int n) {
        assert(n >= 0);
        if (!n) return {};
        assert(!a.empty() && a[0]);
        if (n == 1) return {pw(a[0], P - 2)};
        vector<int> b = inv(a, (n + 1) >> 1);
        int m = 1;
        while (m < n << 1) m <<= 1;
        vector<int> c(a.begin(), a.begin() + min<int>(a.size(), n));
        c.resize(m);
        b.resize(m);
        ntt(c, false);
        ntt(b, false);
        for (int i = 0; i < m; ++i) {
            b[i] = (2 * b[i] - c[i] * b[i] % P * b[i] % P + P) % P;
        }
        ntt(b, true);
        b.resize(n);
        return b;
    }

    static vector<int> div(const vector<int>& a, const vector<int>& b) {
        assert(!b.empty() && b.back());
        int n = a.size(), m = b.size();
        if (n < m) return {0};
        vector<int> x(a.rbegin(), a.rend()), y(b.rbegin(), b.rend());
        y = inv(y, n - m + 1);
        vector<int> q = mul(x, y);
        q.resize(n - m + 1);
        reverse(q.begin(), q.end());
        return q;
    }

    static vector<int> mod(const vector<int>& a, const vector<int>& b) {
        vector<int> r = sub(a, mul(div(a, b), b));
        r.resize(min<int>(r.size(), b.size() - 1));
        return r;
    }

    static vector<int> der(const vector<int>& a) {
        int n = a.size();
        if (n <= 1) return {};
        vector<int> b(n - 1);
        for (int i = 1; i < n; ++i) b[i - 1] = a[i] * i % P;
        return b;
    }

    static vector<int> integ(const vector<int>& a) {
        int n = a.size();
        vector<int> b(n + 1), iv(n + 1);
        if (n) iv[1] = 1;
        for (int i = 2; i <= n; ++i) iv[i] = P - (P / i) * iv[P % i] % P;
        for (int i = 0; i < n; ++i) b[i + 1] = a[i] * iv[i + 1] % P;
        return b;
    }

    static vector<int> ln(const vector<int>& a, int n) {
        assert(n >= 0);
        if (!n) return {};
        assert(!a.empty() && a[0]);
        vector<int> b = integ(mul(der(a), inv(a, n)));
        b.resize(n);
        return b;
    }

    static vector<int> exp(const vector<int>& a, int n) {
        assert(n >= 0);
        if (!n) return {};
        assert(a.empty() || !a[0]);
        vector<int> b{1};
        for (int k = 2; k < n << 1; k <<= 1) {
            int m = min(k, n);
            vector<int> c(a.begin(), a.begin() + min<int>(a.size(), m));
            c.resize(m);
            c = sub(c, ln(b, m));
            ++c[0];
            if (c[0] == P) c[0] = 0;
            b = mul(b, c);
            b.resize(m);
        }
        b.resize(n);
        return b;
    }

    static void build(vector<vector<int>>& t, const vector<int>& x, int u, int l, int r) {
        if (r - l == 1) {
            t[u] = {x[l] ? P - x[l] : 0, 1};
            return;
        }
        int m = (l + r) >> 1;
        build(t, x, u << 1, l, m);
        build(t, x, u << 1 | 1, m, r);
        t[u] = mul(t[u << 1], t[u << 1 | 1]);
    }

    static void eval0(const vector<int>& f, const vector<vector<int>>& t, vector<int>& y, int u, int l, int r) {
        if (r - l == 1) {
            if (f.empty()) y[l] = 0;
            else if (f.size() == 1) y[l] = f[0];
            else y[l] = mod(f, t[u])[0];
            return;
        }
        vector<int> g = mod(f, t[u]);
        int m = (l + r) >> 1;
        eval0(g, t, y, u << 1, l, m);
        eval0(g, t, y, u << 1 | 1, m, r);
    }

    static vector<int> eval(const vector<int>& f, const vector<int>& x) {
        int n = x.size();
        if (!n) return {};
        vector<vector<int>> t(n << 2 | 1);
        build(t, x, 1, 0, n);
        vector<int> y(n);
        eval0(f, t, y, 1, 0, n);
        return y;
    }

    static vector<int> interp(const vector<int>& x, const vector<int>& y) {
        int n = x.size();
        assert(y.size() == n);
        if (!n) return {};
        vector<vector<int>> t(n << 2 | 1);
        build(t, x, 1, 0, n);
        vector<int> d = eval(der(t[1]), x);
        vector<int> w(n);
        for (int i = 0; i < n; ++i) w[i] = y[i] * pw(d[i], P - 2) % P;
        function<vector<int>(int, int, int)> dfs = [&](int u, int l, int r) -> vector<int> {
            if (r - l == 1) return {w[l]};
            int m = (l + r) >> 1;
            return add(mul(dfs(u << 1, l, m), t[u << 1 | 1]), mul(dfs(u << 1 | 1, m, r), t[u << 1]));
        };
        return dfs(1, 0, n);
    }

};

using Poly998 = Poly<998244353, 3>;
