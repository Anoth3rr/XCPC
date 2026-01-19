template <ll MOD, ll G>
struct NTT {
    void ntt(vector<int> a, bool invert) {
        int n = a.size();
        static vector<int> rev;
        static vector<int> roots{0, 1};

        if (rev.size() != n) {
            int k = __builtin_ctz(n);
            rev.assign(n, 0);
            for (int i = 0; i < n; i++)
                rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (k - 1));
        }

        if (roots.size() < n) {
            int k = __builtin_ctz(roots.size());
            roots.resize(n);
            while ((1 << k) < n) {
                int z = qpow(G, (MOD - 1) >> (k + 1));
                for (int i = 1 << (k - 1); i < (1 << k); i++) {
                    roots[i << 1] = roots[i];
                    roots[i << 1 | 1] = 1ll * roots[i] * z % MOD;
                }
                k++;
            }
        }

        for (int i = 0; i < n; i++)
            if (i < rev[i])
                swap(a[i], a[rev[i]]);

        for (int len = 1; len < n; len <<= 1) {
            for (int i = 0; i < n; i += len << 1) {
                for (int j = 0; j < len; j++) {
                    int u = a[i + j];
                    int v = 1ll * a[i + j + len] * roots[len + j] % MOD;
                    a[i + j] = (u + v) % MOD;
                    a[i + j + len] = (u - v + MOD) % MOD;
                }
            }
        }

        if (invert) {
            reverse(a.begin() + 1, a.end());
            int inv = inv(n, MOD);
            for (int &x : a)
                x = 1ll * x * inv % MOD;
        }
    }

    vector<int> multiply(vector<int> a, vector<int> b) {
        int n = 1;
        while (n < (int)a.size() + (int)b.size())
            n <<= 1;
        a.resize(n);
        b.resize(n);
        ntt(a, false);
        ntt(b, false);
        for (int i = 0; i < n; i++)
            a[i] = 1ll * a[i] * b[i] % MOD;
        ntt(a, true);
        return a;
    }
};
