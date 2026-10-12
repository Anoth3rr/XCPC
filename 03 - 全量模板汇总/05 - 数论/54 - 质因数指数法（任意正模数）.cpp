struct FactorComb {
    int n;
    vector<int> primes, spf;
    FactorComb(int n) : n(n), spf(n + 1) {
        for (int i = 2; i <= n; i++) {
            if (!spf[i]) spf[i] = i, primes.push_back(i);
            for (auto p : primes) {
                if (p > spf[i] || p > n / i) break;
                spf[i * p] = p;
            }
        }
    }
    int C(int x, int y, int mod) const {
        assert(mod > 0 && x <= n);
        if (x < 0 || y < 0 || y > x || mod == 1) return 0;
        auto count = [](int x, int p) {
            int r = 0;
            for (; x; x /= p) r += x / p;
            return r;
        };
        auto power = [&](int a, int b) {
            int r = 1 % mod;
            a %= mod;
            for (; b; b >>= 1, a = i128(a) * a % mod)
                if (b & 1) r = i128(r) * a % mod;
            return r;
        };
        int r = 1 % mod;
        for (auto p : primes) {
            if (p > x) break;
            int e = count(x, p) - count(y, p) - count(x - y, p);
            r = i128(r) * power(p, e) % mod;
        }
        return r;
    }
};
