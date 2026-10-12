struct MTT {
    using Z = MInt<0>;

  private:
    using Z0 = MInt<1>;
    using Z1 = MInt<2>;
    using Z2 = MInt<3>;
    static constexpr int p0 = 1004535809;
    static constexpr int p1 = 998244353;
    static constexpr int p2 = 985661441;

    template <class T> static void ntt(vector<T> &a, bool iv) {
        int n = a.size();
        assert(n && !(n & (n - 1)) && (T::getMod() - 1) % n == 0);

        for (int i = 1, j = 0; i < n; i++) {
            int k = n >> 1;
            for (; j & k; k >>= 1) j ^= k;
            j ^= k;
            if (i < j) swap(a[i], a[j]);
        }

        for (int len = 2; len <= n; len <<= 1) {
            T wn = T(3).pow((T::getMod() - 1) / len);
            if (iv) wn = wn.inv();
            for (int i = 0; i < n; i += len) {
                T w = 1;
                for (int j = 0; j < len / 2; j++) {
                    T x = a[i + j], y = a[i + j + len / 2] * w;
                    a[i + j] = x + y;
                    a[i + j + len / 2] = x - y;
                    w *= wn;
                }
            }
        }

        if (iv) {
            T invn = T(n).inv();
            for (T &x : a) x *= invn;
        }
    }

    template <class T> static vector<T> mul0(const vector<Z> &a, const vector<Z> &b) {
        int m = a.size() + b.size() - 1, n = 1;
        while (n < m) n <<= 1;

        vector<T> x(n), y(n);
        for (int i = 0; i < a.size(); i++) x[i] = T(a[i].val());
        for (int i = 0; i < b.size(); i++) y[i] = T(b[i].val());

        bool same = x == y;
        ntt(x, false);
        if (same) {
            for (T &v : x) v *= v;
        } else {
            ntt(y, false);
            for (int i = 0; i < n; i++) x[i] *= y[i];
        }
        ntt(x, true);
        x.resize(m);
        return x;
    }

  public:
    static vector<Z> mul(vector<Z> a, vector<Z> b) {
        int n = a.size(), m = b.size();
        if (!n || !m) return {};
        assert(Z::getMod() > 1);

        if (min(n, m) <= 64) {
            vector<Z> c(n + m - 1);
            for (int i = 0; i < n; i++)
                for (int j = 0; j < m; j++) c[i + j] += a[i] * b[j];
            return c;
        }

        int cnt = 1;
        while (cnt < n + m - 1) cnt <<= 1;
        assert(cnt <= (1LL << 21));

        Z0::setMod(p0);
        Z1::setMod(p1);
        Z2::setMod(p2);

        auto c0 = mul0<Z0>(a, b);
        auto c1 = mul0<Z1>(a, b);
        auto c2 = mul0<Z2>(a, b);

        static const Z1 iv01 = Z1(p0).inv();
        static const Z2 iv02 = Z2(p0).inv();
        static const Z2 iv12 = Z2(p1).inv();

        Z w1 = Z(p0), w2 = w1 * Z(p1);
        vector<Z> c(n + m - 1);
        for (int i = 0; i < n + m - 1; i++) {
            Z1 x1 = (c1[i] - Z1(c0[i].val())) * iv01;
            Z2 x2 = ((c2[i] - Z2(c0[i].val())) * iv02 - Z2(x1.val())) * iv12;

            c[i] = Z(c0[i].val()) + Z(x1.val()) * w1 + Z(x2.val()) * w2;
        }
        return c;
    }
};
