
struct FFT {
    using cd = Complex<double>;
    static constexpr double PI = 3.141592653589793238462643383279502884;

    void fft(vector<cd> &a, bool inv) {
        int n = a.size();
        static vector<int> rev;
        static vector<cd> rts{{0, 0}, {1, 0}};

        if (rev.size() != n) {
            int k = __builtin_ctz(n);
            rev.assign(n, 0);
            for (int i = 0; i < n; i++) rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (k - 1));
        }

        if (rts.size() < n) {
            int k = __builtin_ctz(rts.size());
            rts.resize(n);
            while ((1 << k) < n) {
                double da = 2 * PI / (1 << (k + 1));
                for (int i = 1 << (k - 1); i < (1 << k); i++) {
                    rts[i << 1] = rts[i];
                    double ag = da * (2 * i + 1 - (1 << k));
                    rts[i << 1 | 1] = cd(cos(ag), sin(ag));
                }
                k++;
            }
        }

        for (int i = 0; i < n; i++)
            if (i < rev[i]) swap(a[i], a[rev[i]]);

        for (int len = 1; len < n; len <<= 1) {
            for (int i = 0; i < n; i += len << 1) {
                for (int j = 0; j < len; j++) {
                    cd u = a[i + j];
                    cd v = a[i + j + len] * rts[len + j];
                    a[i + j] = u + v;
                    a[i + j + len] = u - v;
                }
            }
        }

        if (inv) {
            reverse(a.begin() + 1, a.end());
            for (cd &x : a) x /= n;
        }
    }

    vector<int> mul(const vector<int> &a, const vector<int> &b) {
        if (a.empty() || b.empty()) return {};
        int nd = a.size() + b.size() - 1;
        vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
        int n = 1;
        while (n < nd) n <<= 1;
        fa.resize(n);
        fb.resize(n);
        fft(fa, false);
        fft(fb, false);
        for (int i = 0; i < n; i++) fa[i] *= fb[i];
        fft(fa, true);

        vector<int> res(nd);
        for (int i = 0; i < nd; i++) res[i] = llround(fa[i].a);
        return res;
    }
};
