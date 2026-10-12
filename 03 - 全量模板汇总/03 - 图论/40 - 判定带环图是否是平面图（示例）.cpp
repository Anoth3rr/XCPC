signed main() {
    int n, m;
    cin >> n >> m;
    vector<pair<int, int>> in(m);
    for (int i = 0, x, y; i < m; i++) {
        cin >> x >> y;
        in[i] = minmax(x, y);
    }
    TwoSat sat(m);
    for (int i = 0; i < m; i++) {
        auto [s, e] = in[i];
        for (int j = i + 1; j < m; j++) {
            auto [S, E] = in[j];
            if (s < S && S < e && e < E || S < s && s < E && E < e) {
                sat.add(i, 0, j, 0);
                sat.add(i, 1, j, 1);
            }
        }
    }
    if (!sat.work()) {
        cout << "Impossible\n";
        return 0;
    }
    auto ans = sat.answer();
    for (auto it : ans) {
        cout << (it ? "out" : "in") << " ";
    }
}
