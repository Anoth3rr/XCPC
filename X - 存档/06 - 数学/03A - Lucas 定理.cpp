/*
用途：小质数 p 下求 C(n,k)，n、k 可很大。
用法：LucasPrime<Z> c(p)；要求 Z::askMod() == p。
*/

template <class Z> struct LucasPrime {
    int p = 0;
    vector<Z> f, g;

    LucasPrime() = default;

    explicit LucasPrime(int p) {
        init(p);
    }

    void init(int x) {
        assert(x >= 2 && Z::askMod() == x);
        p = x;
        f.assign(p, Z(1));
        g.assign(p, Z(1));
        for (int i = 1; i < p; ++i) f[i] = f[i - 1] * i;
        g[p - 1] = f[p - 1].inv();
        for (int i = p - 1; i; --i) g[i - 1] = g[i] * i;
    }

    Z smallC(int n, int k) const {
        if (k < 0 || k > n) return Z(0);
        return f[n] * g[k] * g[n - k];
    }

    Z C(int n, int k) const {
        assert(p);
        if (k < 0 || k > n) return Z(0);
        Z r = 1;
        while (n || k) {
            int x = n % p, y = k % p;
            if (y > x) return Z(0);
            r *= smallC(x, y);
            n /= p;
            k /= p;
        }
        return r;
    }
};
