vector<Z> divSparse(vector<Z> f, const vector<Z> &g, int n) {
    assert(n >= 0);
    if (!n) return {};
    assert(!g.empty() && g[0] != Z(0));
    f.resize(n);
    vector<pair<int, Z>> a;
    for (int i = 1; i < g.size() && i < n; i++)
        if (g[i] != Z(0)) a.push_back({i, g[i]});
    Z iv = g[0].inv();
    for (int i = 0; i < n; i++) {
        for (auto [j, x] : a) {
            if (j > i) break;
            f[i] -= x * f[i - j];
        }
        f[i] *= iv;
    }
    return f;
}
