/*
用途：轻量 NTT 多项式（乘法、求逆、ln、exp）。
用法：using P = Poly<Z, 3>；Z 为任意 MInt<P>，当前模数须为 NTT 友好质数。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <class Z, int G = 3> struct Poly {
    static vector<Z> cut(vector<Z> a, int n) {
        a.resize(n);
        return a;
    }

    static vector<Z> add(vector<Z> a, const vector<Z> &b) {
        if (a.size() < b.size()) a.resize(b.size());
        for (int i = 0; i < b.size(); ++i) a[i] += b[i];
        return a;
    }

    static vector<Z> sub(vector<Z> a, const vector<Z> &b) {
        if (a.size() < b.size()) a.resize(b.size());
        for (int i = 0; i < b.size(); ++i) a[i] -= b[i];
        return a;
    }

    static void ntt(vector<Z> &a, bool inv) {
        int n = a.size();
        if (n <= 1) return;
        vector<int> rev(n);
        int lg = __builtin_ctzll(n);
        for (int i = 0; i < n; ++i) {
            rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (lg - 1));
            if (i < rev[i]) swap(a[i], a[rev[i]]);
        }
        for (int len = 2; len <= n; len <<= 1) {
            Z wn = mypow(Z(G), (Z::askMod() - 1) / len);
            if (inv) wn = wn.inv();
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
        if (inv) {
            Z iv = Z(n).inv();
            for (auto &x : a) x *= iv;
        }
    }

    static vector<Z> mul(vector<Z> a, vector<Z> b) {
        if (a.empty() || b.empty()) return {};
        int nd = a.size() + b.size() - 1;
        int n = 1;
        while (n < nd) n <<= 1;
        a.resize(n), b.resize(n);
        ntt(a, false), ntt(b, false);
        for (int i = 0; i < n; ++i) a[i] *= b[i];
        ntt(a, true);
        a.resize(nd);
        return a;
    }

    static vector<Z> der(const vector<Z> &a) {
        if (a.size() <= 1) return {};
        vector<Z> b(a.size() - 1);
        for (int i = 1; i < a.size(); ++i) b[i - 1] = a[i] * Z(i);
        return b;
    }

    static vector<Z> integ(const vector<Z> &a) {
        vector<Z> b(a.size() + 1);
        for (int i = 0; i < a.size(); ++i) b[i + 1] = a[i] / Z(i + 1);
        return b;
    }

    static vector<Z> inv(const vector<Z> &a, int m) {
        assert(m >= 0);
        if (m == 0) return {};
        assert(!a.empty() && a[0] != Z(0));
        vector<Z> b{a[0].inv()};
        for (int k = 2; k < 2 * m; k <<= 1) {
            vector<Z> f = cut(a, min(k, m));
            vector<Z> t = mul(mul(b, b), f);
            b.resize(min(k, m));
            for (int i = 0; i < b.size(); ++i) b[i] += b[i] - t[i];
        }
        b.resize(m);
        return b;
    }

    static vector<Z> ln(const vector<Z> &a, int m) {
        assert(m >= 0);
        if (m == 0) return {};
        assert(!a.empty() && a[0] == Z(1));
        vector<Z> b = integ(mul(der(a), inv(a, m)));
        b.resize(m);
        return b;
    }

    static vector<Z> exp(const vector<Z> &a, int m) {
        assert(m >= 0);
        if (m == 0) return {};
        assert(a.empty() || a[0] == Z(0));
        vector<Z> b{1};
        for (int k = 2; k < 2 * m; k <<= 1) {
            int n = min(k, m);
            vector<Z> t = sub(cut(a, n), ln(b, n));
            t[0] += Z(1);
            b = cut(mul(b, t), n);
        }
        b.resize(m);
        return b;
    }
};
