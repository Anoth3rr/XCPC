struct FFT {
    using cd = complex<double>;

    static void fft(vector<cd> &a, bool iv) {
        int n = a.size();
        assert(n && !(n & (n - 1)));
        if (n == 1) return;

        static vector<int> rev;
        static vector<cd> wn;
        if (rev.size() != n) {
            rev.assign(n, 0);
            for (int i = 1; i < n; i++)
                rev[i] = (rev[i >> 1] >> 1) | ((i & 1) ? n >> 1 : 0);

            wn.resize(n / 2);
            const double PI = acos(-1.0);
            for (int i = 0; i < n / 2; i++)
                wn[i] = polar(1.0, 2 * PI * i / n);
        }

        for (int i = 0; i < n; i++)
            if (i < rev[i]) swap(a[i], a[rev[i]]);

        for (int len = 2; len <= n; len <<= 1) {
            int step = n / len;
            for (int i = 0; i < n; i += len) {
                for (int j = 0; j < len / 2; j++) {
                    cd w = wn[j * step];
                    if (iv) w = conj(w);
                    cd x = a[i + j], y = a[i + j + len / 2] * w;
                    a[i + j] = x + y;
                    a[i + j + len / 2] = x - y;
                }
            }
        }

        if (iv)
            for (cd &x : a) x /= n;
    }
};
