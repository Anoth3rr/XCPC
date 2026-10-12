vector<Z> polyPow(vector<Z> f, const string &k, int n) {
    assert(n >= 0 && !k.empty());
    if (!n) return {};
    int p = Z::getMod(), a = 0, b = 0, e = 0;
    assert(n < p);
    for (char c : k) {
        assert('0' <= c && c <= '9');
        int d = c - '0';
        a = (a * 10 + d) % p;
        b = (b * 10 + d) % (p - 1);
        e = min(n, e * 10 + d);
    }
    vector<Z> r(n);
    if (!e) { r[0] = 1; return r; }
    f.resize(n);
    int t = 0;
    while (t < n && f[t] == Z(0)) t++;
    if (t == n || i128(t) * e >= n) return r;

    int d = t * e, m = n - d;
    Z c = f[t], iv = c.inv();
    vector<Z> g(f.begin() + t, f.end());
    g.resize(m);
    for (Z &x : g) x *= iv;
    g = NTT::ln(g, m);
    for (Z &x : g) x *= Z(a);
    g = NTT::exp(g, m);
    c = power(c, b);
    for (int i = 0; i < m; i++) r[i + d] = g[i] * c;
    return r;
}
