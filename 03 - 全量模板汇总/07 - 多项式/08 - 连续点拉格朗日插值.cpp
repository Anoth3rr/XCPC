Z Lagrange(const vector<Z> &f, Z x) {
    int n = f.size() - 1;
    vector<Z> pre(n + 2, 1);
    for (int i = 0; i <= n; i++) pre[i + 1] = pre[i] * (x - Z(i));
    Z ans = 0, suf = 1;
    for (int i = n; i >= 0; i--) {
        Z v = f[i] * pre[i] * suf * C.ifac(i) * C.ifac(n - i);
        if ((n - i) & 1)
            ans = ans - v;
        else
            ans = ans + v;

        suf = suf * (x - Z(i));
    }
    return ans;
}
