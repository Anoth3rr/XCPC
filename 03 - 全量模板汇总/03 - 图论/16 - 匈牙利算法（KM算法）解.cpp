signed main() {
    int n1, n2, m;
    cin >> n1 >> n2 >> m;

    vector<vector<int>> ver(n1 + 1);
    for (int i = 1; i <= m; ++i) {
        int x, y;
        cin >> x >> y;
        ver[x].push_back(y); //只需要建立单向边
    }

    int ans = 0;
    vector<int> match(n2 + 1);
    for (int i = 1; i <= n1; ++i) {
        vector<int> vis(n2 + 1);
        auto dfs = [&](auto self, int x) -> bool {
            for (auto y : ver[x]) {
                if (vis[y]) continue;
                vis[y] = 1;
                if (!match[y] || self(self, match[y])) {
                    match[y] = x;
                    return true;
                }
            }
            return false;
        };
        if (dfs(dfs, i)) {
            ans++;
        }
    }
    cout << ans << endl;
}
