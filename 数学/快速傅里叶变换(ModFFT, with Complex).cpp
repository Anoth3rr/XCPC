struct ModFFT {
    using cd = Complex<double>;
    using ll = long long;
    const double PI = acos(-1);

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

    vector<int> multiply(const vector<int> &a, const vector<int> &b, int mod) {
        int n = 1;
        while (n < a.size() + b.size())
            n <<= 1;

        vector<cd> fa(n), fb(n);
        for (int i = 0; i < a.size(); i++)
            fa[i] = cd(a[i] & 32767, a[i] >> 15);
        for (int i = 0; i < b.size(); i++)
            fb[i] = cd(b[i] & 32767, b[i] >> 15);

        fft(fa, false);
        fft(fb, false);

        vector<cd> fa_fb(n), fa_fb2(n);
        for (int i = 0; i < n; i++) {
            int j = (n - i) & (n - 1);
            cd a1 = (fa[i] + conj(fa[j])) * cd(0.5, 0);
            cd a2 = (fa[i] - conj(fa[j])) * cd(0, -0.5);
            cd b1 = (fb[i] + conj(fb[j])) * cd(0.5, 0);
            cd b2 = (fb[i] - conj(fb[j])) * cd(0, -0.5);
            fa_fb[i] = a1 * b1 + a2 * b2 * cd(0, 1);
            fa_fb2[i] = a1 * b2 + a2 * b1;
        }

        fft(fa_fb, true);
        fft(fa_fb2, true);

        vector<int> res(n);
        for (int i = 0; i < n; i++) {
            ll x = (ll)(fa_fb[i].a() + 0.5) % mod;
            ll y = (ll)(fa_fb2[i].a() + 0.5) % mod;
            ll z = (ll)(fa_fb[i].b() + 0.5) % mod;
            res[i] = (x + (y << 15) + (z << 30)) % mod;
        }
        return res;
    }
};
