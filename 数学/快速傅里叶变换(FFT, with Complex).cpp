struct FFT {
    using cd = Complex<double>;
    using ll = long long;
    constexpr double PI = acos(-1);

    void fft(vector<cd> &a, bool invert) {
        int n = a.size();
        static vector<int> rev;
        static vector<cd> roots{{0, 0}, {1, 0}};

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
                double angle = 2 * PI / (1 << (k + 1));
                for (int i = 1 << (k - 1); i < (1 << k); i++) {
                    roots[i << 1] = roots[i];
                    double ang = angle * (2 * i + 1 - (1 << k));
                    roots[i << 1 | 1] = cd(cos(ang), sin(ang));
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
                    cd u = a[i + j];
                    cd v = a[i + j + len] * roots[len + j];
                    a[i + j] = u + v;
                    a[i + j + len] = u - v;
                }
            }
        }

        if (invert) {
            reverse(a.begin() + 1, a.end());
            for (cd &x : a)
                x /= n;
        }
    }

    vector<ll> multiply(const vector<int> &a, const vector<int> &b) {
        vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
        int n = 1;
        while (n < a.size() + b.size())
            n <<= 1;
        fa.resize(n);
        fb.resize(n);
        fft(fa, false);
        fft(fb, false);
        for (int i = 0; i < n; i++)
            fa[i] *= fb[i];
        fft(fa, true);

        vector<ll> res(n);
        for (int i = 0; i < n; i++)
            res[i] = (ll)(fa[i].real() + 0.5);
        return res;
    }
};
