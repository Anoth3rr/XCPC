template <int G = 3> struct Poly {
    static void ntt(vector<Z> &a, bool iv) {
        int n = a.size();
        assert(n && !(n & (n - 1)));
        vector<int> rv(n);
        int k = __builtin_ctz(static_cast<unsigned>(n));
        if (n > 1)
            for (int i = 0; i < n; ++i) rv[i] = (rv[i >> 1] >> 1) | ((i & 1) << (k - 1));
        for (int i = 0; i < n; ++i)
            if (i < rv[i]) swap(a[i], a[rv[i]]);
        for (int len = 2; len <= n; len <<= 1) {
            Z wn = power(Z(G), (Z::getMod() - 1) / len);
            if (iv) wn = wn.inv();
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
        if (iv) {
            Z x = Z(n).inv();
            for (Z &v : a) v *= x;
        }
    }
};
