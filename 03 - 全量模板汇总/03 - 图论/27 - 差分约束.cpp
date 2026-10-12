signed main() {
    int n, m;
    cin >> n >> m;
    
    vector<array<int, 3>> e(m + 1);
    for (int i = 1; i <= m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        e[i] = {v, u, w};
    }
    
    vector<int> d(n + 1, 1E9);
    d[1] = 0;
    for (int i = 1; i < n; i++) {
        for (int j = 1; j <= m; j++) {
            auto [u, v, w] = e[j];
            d[v] = min(d[v], d[u] + w);
        }
    }
    for (int i = 1; i <= m; i++) {
        auto [u, v, w] = e[i];
        if (d[v] > d[u] + w) {
            cout << "NO\n";
            return 0;
        }
    }
    for (int i = 1; i <= n; i++) {
        cout << d[i] << " \n"[i == n];
    }
    return 0;
}
