/*
用途：用 NTT 快速求固定 n 的第一类、第二类 Stirling 数整行。
依赖：02A - 自动取模（MInt）、08B 或 08C - 多项式（Poly, Z）。
接口：StirFast<Z>::ask1(n) 返回无符号第一类；ask1(n, true) 返回有符号第一类；
      StirFast<Z>::ask2(n) 返回第二类，返回值下标 k 为 0..n。
复杂度：第一类 O(n log^2 n)，第二类 O(n log n)；均比 03E 的 O(n^2) 适合大 n。
限制：Z 的模数须为 NTT 友好质数，且 n < mod；小 n 使用 03E 常数更小。
*/
template <class Z, int G = 3> struct StirFast {
    using P = Poly<Z, G>;

    static vector<Z> mul(vector<Z> a, vector<Z> b) {
        if (a.empty() || b.empty()) return {};
        int n = a.size(), m = b.size();
        if (min(n, m) <= 32) {
            vector<Z> c(n + m - 1);
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < m; ++j) c[i + j] += a[i] * b[j];
            return c;
        }
        return P::mul(move(a), move(b));
    }

    static vector<Z> prod(int l, int r) {
        if (l >= r) return {Z(1)};
        if (r - l == 1) return {Z(l), Z(1)};
        int m = (l + r) >> 1;
        return mul(prod(l, m), prod(m, r));
    }

    static vector<Z> ask1(int n, bool sg = false) {
        assert(n >= 0);
        vector<Z> a = prod(0, n);
        a.resize(n + 1);
        if (sg)
            for (int k = 0; k <= n; ++k)
                if ((n - k) & 1) a[k] = -a[k];
        return a;
    }

    static Z ask1(int n, int k, bool sg = false) {
        assert(0 <= k && k <= n);
        return ask1(n, sg)[k];
    }

    static vector<Z> ask2(int n) {
        assert(0 <= n && n < Z::askMod());
        vector<Z> f(n + 1), g(n + 1), fac(n + 1), iv(n + 1);
        fac[0] = 1;
        for (int i = 1; i <= n; ++i) fac[i] = fac[i - 1] * Z(i);
        iv[n] = fac[n].inv();
        for (int i = n; i; --i) iv[i - 1] = iv[i] * Z(i);
        for (int i = 0; i <= n; ++i) {
            f[i] = Z(i).pow(n) * iv[i];
            g[i] = (i & 1) ? -iv[i] : iv[i];
        }
        vector<Z> a = mul(move(f), move(g));
        a.resize(n + 1);
        return a;
    }
};
