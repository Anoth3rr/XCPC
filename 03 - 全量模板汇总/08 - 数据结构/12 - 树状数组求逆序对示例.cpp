int solve(const vector<int> &a) {
    int n = a.size() - 1;
    vector<pii> ord;

    for (int i = 1; i <= n; i++) ord.push_back({a[i], i});
    sort(ord.begin(), ord.end());

    BIT tr(n);
    int ans = 0;
    for (auto [v, x] : ord) {
        ans += tr.ask(x + 1, n);
        tr.add(x, 1);
    }
    return ans;
}
