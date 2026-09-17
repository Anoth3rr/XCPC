template <int T, auto F, auto sF, auto fpe, int f1> struct Min25 {
    int n, m, sq;
    vector<int> id1, id2, val;
    vector<array<Z, T>> g, sFp;
    array<int, T> a;
    int id(int x) {
        return x <= sq ? id1[x] : id2[n / x];
    }

    Min25(int n, array<int, T> a) : n(n), a(a) {
        sq = sqrt(n);
        while ((sq + 1) * (sq + 1) <= n) sq++;
        while (sq * sq > n) sq--;
        id1.assign(sq + 1, -1);
        id2.assign(sq + 1, -1);
        for (int l = 1; l <= n;) {
            int x = n / l, r = n / x;
            int pos = val.size();
            val.push_back(x);
            if (x <= sq)
                id1[x] = pos;
            else
                id2[n / x] = pos;
            if (r == n) break;
            l = r + 1;
        }
        m = val.size();
        g.assign(m, array<Z, T>{});
        sFp.assign(primes.size() + 1, array<Z, T>{});
        for (int j = 0; j < T; ++j) {
            for (int i = 0; i < m; ++i) g[i][j] = sF(val[i], j) - F(1, j);
            for (int i = 0; i < primes.size(); ++i) sFp[i + 1][j] = sFp[i][j] + F(primes[i], j);
            for (int i = 0; i < primes.size() && primes[i] * primes[i] <= n; ++i) {
                int p = primes[i];
                Z fp = F(p, j);
                for (int k = 0; k < m && p * p <= val[k]; ++k) {
                    int q = id(val[k] / p);
                    g[k][j] -= fp * (g[q][j] - sFp[i][j]);
                }
            }
        }
    }

    Z P(int n, int j) {
        return g[id(n)][j];
    }

    Z S(int x, int y) {
        if (x <= 1 || (y && primes[y - 1] >= x)) return 0;
        Z ans = 0;
        for (int j = 0; j < T; ++j) ans += a[j] * (g[id(x)][j] - sFp[y][j]);
        for (int i = y; i < primes.size() && primes[i] <= x / primes[i]; ++i) {
            int p = primes[i], pe = p;
            for (int e = 1; pe <= x; ++e) {
                ans += fpe(p, e, pe) * (S(x / pe, i + 1) + (e != 1));
                if (pe > x / p) break;
                pe *= p;
            }
        }
        return ans;
    }

    Z sf(int n) {
        return f1 + S(n, 0);
    }
};

Z F(int x, int j) {
    return x;
}

Z sF(int n, int j) {
    return Z(n + 1) * n / 2;
}

Z fpe(int p, int e, int pe) {
    return pe;
}