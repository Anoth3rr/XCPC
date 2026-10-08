int exL(int n, int m, int mod) {
    if (m < 0 || m > n || mod == 1) return 0;
    int ans = 0, x = mod;
    auto solve = [&](int p, int pk) {
        vector<int> f(pk + 1, 1);
        for (int i = 1; i <= pk; ++i) f[i] = mul(f[i - 1], i % p ? i : 1, pk);

        auto fac = [&](auto &&self, int x) -> int {
            if (!x) return 1;
            return mul(mul(power(f[pk], x / pk, pk), f[x % pk], pk), self(self, x / p), pk);
        };

        int e = 0, phi = pk / p * (p - 1);
        for (int y = n; y; y /= p) e += y / p;
        for (int y = m; y; y /= p) e -= y / p;
        for (int y = n - m; y; y /= p) e -= y / p;

        int den = mul(fac(fac, m), fac(fac, n - m), pk);
        int r = mul(mul(fac(fac, n), power(den, phi - 1, pk), pk), power(p, e, pk), pk);
        int M = mod / pk;
        int add = mul(mul(r, M, mod), power(M, phi - 1, pk), mod);
        ans = (ans + add) % mod;
    };
    for (int p = 2; p <= x / p; ++p) {
        if (x % p) continue;
        int pk = 1;
        while (x % p == 0) x /= p, pk *= p;
        solve(p, pk);
    }
    if (x > 1) solve(x, x);
    return ans;
}