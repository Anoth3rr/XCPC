/*
用途：在自动取模类型 Z 上预处理伯努利数，并 O(k) 求 1^k+...+n^k。
约定：B1=-1/2；Z 需支持构造、加减乘除和 pow/inv；模数应满足除法前提。
接口：Bernoulli<Z> b(d)，b.ask(k,n) 求前 n 项，b.ask(k,l,r) 求闭区间和。
复杂度：预处理 O(d^2)，单次幂和 O(k)，空间 O(d)。
*/
template <class Z> struct Bernoulli {
    int d;
    vector<Z> b, fac, iv;

    explicit Bernoulli(int _d) : d(_d), b(_d + 2), fac(_d + 3), iv(_d + 3) {
        assert(d >= 0);
        fac[0] = 1;
        for (int i = 1; i <= d + 2; ++i) fac[i] = fac[i - 1] * Z(i);
        iv[d + 2] = fac[d + 2].inv();
        for (int i = d + 2; i; --i) iv[i - 1] = iv[i] * Z(i);
        b[0] = 1;
        for (int i = 1; i <= d + 1; ++i) {
            Z s = 0;
            for (int j = 0; j < i; ++j) s += C(i + 1, j) * b[j];
            b[i] = -s / Z(i + 1);
        }
    }

    Z C(int n, int k) const {
        if (k < 0 || k > n) return 0;
        return fac[n] * iv[k] * iv[n - k];
    }

    // 1^k + 2^k + ... + n^k，n=0 时返回 0。
    Z ask(int k, int n) const {
        assert(0 <= k && k <= d && n >= 0);
        vector<Z> pw(k + 2);
        pw[0] = 1;
        Z x = Z(n + 1);
        for (int i = 1; i <= k + 1; ++i) pw[i] = pw[i - 1] * x;
        Z s = 0, q = 0;
        for (int j = 0; j <= k + 1; ++j) {
            Z c = C(k + 1, j) * b[j];
            s += c * pw[k + 1 - j];
            q += c;
        }
        return (s - q) / Z(k + 1);
    }

    // l^k + (l+1)^k + ... + r^k，要求 1<=l<=r。
    Z ask(int k, int l, int r) const {
        assert(1 <= l && l <= r);
        return ask(k, r) - ask(k, l - 1);
    }
};
