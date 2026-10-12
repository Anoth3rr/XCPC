template <int G = 3> struct Poly {
    static void ntt(vector<Z> &a, bool iv) {
        int n = a.size();
        assert(n && !(n & (n - 1)));
        vector<int> rv(n);
        int k = __builtin_ctz(static_cast<unsigned>(n));
        if (n > 1)
            for (int i = 0; i < n; ++i) rv[i] = (rv[i >> 1] >> 1) | ((i & 1) << (k - 1));
        for (int i = 0; i < n; ++i)
            if (i < rv[i]) swap(a[i], a[rv[i]]);
        for (int len = 2; len <= n; len <<= 1) {
            Z wn = power(Z(G), (Z::getMod() - 1) / len);
            if (iv) wn = wn.inv();
            for (int i = 0; i < n; i += len) {
                Z w = 1;
                for (int j = 0; j < len / 2; ++j) {
                    Z x = a[i + j], y = a[i + j + len / 2] * w;
                    a[i + j] = x + y;
                    a[i + j + len / 2] = x - y;
                    w *= wn;
                }
            }
        }
        if (iv) {
            Z x = Z(n).inv();
            for (Z &v : a) v *= x;
        }
    }

    static vector<Z> add(vector<Z> a, vector<Z> b) {
        int n = max(a.size(), b.size());
        a.resize(n), b.resize(n);
        for (int i = 0; i < n; ++i) a[i] += b[i];
        return a;
    }

    static vector<Z> sub(vector<Z> a, vector<Z> b) {
        int n = max(a.size(), b.size());
        a.resize(n), b.resize(n);
        for (int i = 0; i < n; ++i) a[i] -= b[i];
        return a;
    }

    static vector<Z> mul(vector<Z> a, vector<Z> b) {
        if (a.empty() || b.empty()) return {};
        int m = a.size() + b.size() - 1, n = 1;
        while (n < m) n <<= 1;
        a.resize(n), b.resize(n);
        ntt(a, false), ntt(b, false);
        for (int i = 0; i < n; ++i) a[i] *= b[i];
        ntt(a, true);
        a.resize(m);
        return a;
    }

    static vector<Z> inv(const vector<Z> &a, int n) {
        assert(n >= 0);
        if (!n) return {};
        assert(!a.empty() && a[0] != Z(0));
        if (n == 1) return {a[0].inv()};
        vector<Z> b = inv(a, (n + 1) >> 1);
        int m = 1;
        while (m < n << 1) m <<= 1;
        vector<Z> c(a.begin(), a.begin() + min<int>(a.size(), n));
        c.resize(m), b.resize(m);
        ntt(c, false), ntt(b, false);
        for (int i = 0; i < m; ++i) b[i] *= Z(2) - c[i] * b[i];
        ntt(b, true);
        b.resize(n);
        return b;
    }

    static vector<Z> div(const vector<Z> &a, const vector<Z> &b) {
        assert(!b.empty() && b.back() != Z(0));
        int n = a.size(), m = b.size();
        if (n < m) return {Z(0)};
        vector<Z> x(a.rbegin(), a.rend()), y(b.rbegin(), b.rend());
        y = inv(y, n - m + 1);
        vector<Z> q = mul(x, y);
        q.resize(n - m + 1);
        reverse(q.begin(), q.end());
        return q;
    }

    static vector<Z> mod(const vector<Z> &a, const vector<Z> &b) {
        vector<Z> r = sub(a, mul(div(a, b), b));
        r.resize(min<int>(r.size(), b.size() - 1));
        return r;
    }

    static vector<Z> der(const vector<Z> &a) {
        int n = a.size();
        if (n <= 1) return {};
        vector<Z> b(n - 1);
        for (int i = 1; i < n; ++i) b[i - 1] = a[i] * Z(i);
        return b;
    }

    static vector<Z> integ(const vector<Z> &a) {
        int n = a.size();
        vector<Z> b(n + 1);
        for (int i = 0; i < n; ++i) b[i + 1] = a[i] / Z(i + 1);
        return b;
    }

    static vector<Z> ln(const vector<Z> &a, int n) {
        assert(n >= 0);
        if (!n) return {};
        assert(!a.empty() && a[0] != Z(0));
        vector<Z> b = integ(mul(der(a), inv(a, n)));
        b.resize(n);
        return b;
    }

    static vector<Z> exp(const vector<Z> &a, int n) {
        assert(n >= 0);
        if (!n) return {};
        assert(a.empty() || a[0] == Z(0));
        vector<Z> b{Z(1)};
        for (int k = 2; k < n << 1; k <<= 1) {
            int m = min(k, n);
            vector<Z> c(a.begin(), a.begin() + min<int>(a.size(), m));
            c.resize(m);
            c = sub(c, ln(b, m));
            c[0] += Z(1);
            b = mul(b, c);
            b.resize(m);
        }
        b.resize(n);
        return b;
    }

    static void build(vector<vector<Z>> &t, const vector<Z> &x, int u, int l, int r) {
        if (r - l == 1) {
            t[u] = {-x[l], Z(1)};
            return;
        }
        int m = (l + r) >> 1;
        build(t, x, u << 1, l, m);
        build(t, x, u << 1 | 1, m, r);
        t[u] = mul(t[u << 1], t[u << 1 | 1]);
    }

    static void eval0(const vector<Z> &f, const vector<vector<Z>> &t, vector<Z> &y, int u, int l, int r) {
        if (r - l == 1) {
            if (f.empty())
                y[l] = Z(0);
            else if (f.size() == 1)
                y[l] = f[0];
            else
                y[l] = mod(f, t[u])[0];
            return;
        }
        vector<Z> g = mod(f, t[u]);
        int m = (l + r) >> 1;
        eval0(g, t, y, u << 1, l, m);
        eval0(g, t, y, u << 1 | 1, m, r);
    }

    static vector<Z> eval(const vector<Z> &f, const vector<Z> &x) {
        int n = x.size();
        if (!n) return {};
        vector<vector<Z>> t(n << 2 | 1);
        build(t, x, 1, 0, n);
        vector<Z> y(n);
        eval0(f, t, y, 1, 0, n);
        return y;
    }

    static vector<Z> interp(const vector<Z> &x, const vector<Z> &y) {
        int n = x.size();
        assert(y.size() == n);
        if (!n) return {};
        vector<vector<Z>> t(n << 2 | 1);
        build(t, x, 1, 0, n);
        vector<Z> d = eval(der(t[1]), x), w(n);
        for (int i = 0; i < n; ++i) w[i] = y[i] / d[i];
        function<vector<Z>(int, int, int)> dfs = [&](int u, int l, int r) -> vector<Z> {
            if (r - l == 1) return {w[l]};
            int m = (l + r) >> 1;
            return add(mul(dfs(u << 1, l, m), t[u << 1 | 1]), mul(dfs(u << 1 | 1, m, r), t[u << 1]));
        };
        return dfs(1, 0, n);
    }

    static vector<Z> shift(vector<Z> a, int k) {
        int n = a.size();
        if (k >= 0) a.insert(a.begin(), k, Z(0));
        else if (k <= -n) return {};
        else a.erase(a.begin(), a.begin() - k);
        return a;
    }
    static vector<Z> trunc(vector<Z> a, int n) {
        assert(n >= 0);
        a.resize(n);
        return a;
    }
    static vector<Z> mulT(const vector<Z> &a, vector<Z> b) {
        if (a.empty() || b.empty()) return {};
        int n = b.size();
        reverse(b.begin(), b.end());
        return shift(mul(a, b), 1 - n);
    }
    static vector<Z> pow(const vector<Z> &a, int k, int n) {
        assert(k >= 0 && n >= 0);
        vector<Z> r(n);
        if (!n) return r;
        if (!k) { r[0] = 1; return r; }
        int i = 0;
        while (i < a.size() && a[i] == Z(0)) ++i;
        if (i == a.size() || i128(i) * k >= n) return r;
        int d = i * k, m = n - d;
        Z v = a[i];
        vector<Z> b(a.begin() + i, a.end());
        b.resize(m);
        Z iv = v.inv();
        for (auto &x : b) x *= iv;
        b = ln(b, m);
        for (auto &x : b) x *= Z(k);
        b = exp(b, m);
        v = power(v, k);
        for (int j = 0; j < m; ++j) r[j + d] = b[j] * v;
        return r;
    }
    static vector<Z> sqrt(const vector<Z> &a, int n) {
        assert(n >= 0);
        if (!n) return {};
        assert(!a.empty() && a[0] == Z(1));
        vector<Z> b{Z(1)};
        Z half = Z(2).inv();
        for (int k = 2; k < n * 2; k *= 2) {
            int m = min(k, n);
            auto c = trunc(mul(trunc(a, m), inv(b, m)), m);
            b.resize(m);
            for (int i = 0; i < m; i++) b[i] = (b[i] + c[i]) * half;
        }
        return trunc(b, n);
    }
};

using NTT = Poly<3>;
