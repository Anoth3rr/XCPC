vector<Z> powSparse(vector<Z> f, int k, int n) {
    assert(k >= 0 && n >= 0 && n < Z::getMod());
    vector<Z> r(n);
    if (!n) return r;
    if (!k) { r[0] = 1; return r; }
    f.resize(n);
    int t = 0;
    while (t < n && f[t] == Z(0)) t++;
    if (t == n || i128(t) * k >= n) return r;

    int d = t * k, m = n - d;
    Z iv = f[t].inv(), e = Z(k) + Z(1);
    vector<pair<int, Z>> a;
    for (int i = 1; i < m && t + i < n; i++)
        if (f[t + i] != Z(0)) a.push_back({i, f[t + i] * iv});
    C.init(m - 1);
    r[d] = power(f[t], k);
    for (int i = 1; i < m; i++) {
        for (auto [j, x] : a) {
            if (j > i) break;
            r[d + i] += (e * Z(j) - Z(i)) * x * r[d + i - j];
        }
        r[d + i] *= C.inv(i);
    }
    return r;
}
