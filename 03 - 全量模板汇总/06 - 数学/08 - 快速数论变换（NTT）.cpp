template <int P, int G> struct NTT {
    static int pow(int a, int n) {
        int r = 1;
        while (n) {
            if (n & 1) r = r * a % P;
            a = a * a % P;
            n >>= 1;
        }
        return r;
    }

    static void ntt(vector<int> &a, bool iv) {
        int n = a.size();
        assert(n && !(n & (n - 1)));
        static vector<int> rv;
        static vector<int> rt{0, 1};
        if (rv.size() != n) {
            int k = __builtin_ctz(static_cast<unsigned>(n));
            rv.assign(n, 0);
            if (n > 1)
                for (int i = 0; i < n; ++i) rv[i] = (rv[i >> 1] >> 1) | ((i & 1) << (k - 1));
        }
        if (rt.size() < n) {
            int k = __builtin_ctz(static_cast<unsigned>(rt.size()));
            rt.resize(n);
            while ((1LL << k) < n) {
                int z = pow(G, (P - 1) >> (k + 1));
                for (int i = 1LL << (k - 1); i < (1LL << k); ++i) {
                    rt[i << 1] = rt[i];
                    rt[i << 1 | 1] = rt[i] * z % P;
                }
                ++k;
            }
        }
        for (int i = 0; i < n; ++i)
            if (i < rv[i]) swap(a[i], a[rv[i]]);
        for (int len = 1; len < n; len <<= 1) {
            for (int i = 0; i < n; i += len << 1) {
                for (int j = 0; j < len; ++j) {
                    int u = a[i + j];
                    int v = a[i + j + len] * rt[len + j] % P;
                    a[i + j] = (u + v) % P;
                    a[i + j + len] = (u - v + P) % P;
                }
            }
        }
        if (iv) {
            reverse(a.begin() + 1, a.end());
            int x = pow(n, P - 2);
            for (int &v : a) v = v * x % P;
        }
    }

    static vector<int> mul(vector<int> a, vector<int> b) {
        if (a.empty() || b.empty()) return {};
        int m = a.size() + b.size() - 1;
        int n = 1;
        while (n < m) n <<= 1;
        a.resize(n);
        b.resize(n);
        ntt(a, false);
        ntt(b, false);
        for (int i = 0; i < n; ++i) a[i] = a[i] * b[i] % P;
        ntt(a, true);
        a.resize(m);
        return a;
    }
};
