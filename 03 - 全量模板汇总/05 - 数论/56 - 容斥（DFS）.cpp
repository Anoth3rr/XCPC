void solve() {
    int n;
    cin >> n;
    vector<int> p(n);
    for (auto &v : p) cin >> v;

    int ans = 0;
    auto dfs = [&](auto &&self, int i, int x, int cnt) -> void {
        if (i == n) {
            if (cnt) ans += (cnt & 1 ? 1 : -1) * (n / x);
            return;
        }
        self(self, i + 1, x, cnt);
        if (x <= n / p[i]) self(self, i + 1, x * p[i], cnt + 1);
    };
    dfs(dfs, 0, 1, 0);
    cout << ans << endl;
}
