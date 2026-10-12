vector<Z> shiftValues(const vector<Z> &f, Z c) {
    int n = f.size();
    if (!n) return {};
    assert(n < Z::getMod());
    C.init(n - 1);
    vector<Z> a(n), b(n * 2 - 1), pre(n * 2, 1);
    for (int i = 0; i < n; i++) {
        a[i] = f[i] * C.ifac(i) * C.ifac(n - 1 - i);
        if ((n - 1 - i) & 1) a[i] = -a[i];
    }
    for (int i = 0; i < n * 2 - 1; i++) {
        b[i] = c + Z(i - n + 1);
        if (b[i] == Z(0)) b[i] = 1;
        pre[i + 1] = pre[i] * b[i];
    }
    Z iv = pre.back().inv();
    for (int i = n * 2 - 2; i >= 0; i--) {
        Z x = b[i];
        b[i] = iv * pre[i];
        iv *= x;
    }
    auto h = NTT::mul(a, b);
    vector<Z> r(n);
    Z w = pre[n];
    for (int i = 0; i < n; i++) {
        int t = (c + Z(i)).val();
        r[i] = t < n ? f[t] : h[n - 1 + i] * w;
        if (i + 1 < n) {
            Z x = c + Z(i + 1);
            if (x == Z(0)) x = 1;
            w *= x * b[i];
        }
    }
    return r;
}
