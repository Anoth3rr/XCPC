/*
用途：阶乘、逆阶乘与 C(n,k)、A(n,k)。
用法：Comb<Z> c(n)；Z 可为任意 MInt<P>。
前提：模数为质数，且查询 n < mod；首次扩展 O(n)，之后 O(1)。
*/
template <class Z> struct Comb {
    int n = 0;
    vector<Z> f{Z(1)}, g{Z(1)};

    Comb() = default;
    explicit Comb(int m) {
        init(m);
    }

    void init(int m) {
        assert(m >= 0 && m < Z::askMod());
        if (m <= n) return;
        f.resize(m + 1);
        g.resize(m + 1);
        for (int i = n + 1; i <= m; ++i) f[i] = f[i - 1] * i;
        g[m] = f[m].inv();
        for (int i = m; i > n; --i) g[i - 1] = g[i] * i;
        n = m;
    }

    Z fac(int x) {
        assert(x >= 0);
        if (x > n) init(x);
        return f[x];
    }

    Z ifac(int x) {
        assert(x >= 0);
        if (x > n) init(x);
        return g[x];
    }

    Z C(int x, int k) {
        if (x < 0 || k < 0 || k > x) return Z(0);
        return fac(x) * ifac(k) * ifac(x - k);
    }

    Z A(int x, int k) {
        if (x < 0 || k < 0 || k > x) return Z(0);
        return fac(x) * ifac(x - k);
    }
};

template <class Z> using Comb = Comb<Z>;
