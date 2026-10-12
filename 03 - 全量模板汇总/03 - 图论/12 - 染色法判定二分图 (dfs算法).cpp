vector<int> vis(n + 1);
auto dfs = [&](auto self, int x, int type) -> void {
    vis[x] = type;
    for (auto y : ver[x]) {
        if (vis[y] == type) {
            cout << "NO\n";
            exit(0);
        }
        if (vis[y]) continue;
        self(self, y, 3 - type);
    }
};
for (int i = 1; i <= n; ++i) {
    if (vis[i]) {
        dfs(dfs, i, 1);
    }
}
cout << "Yes\n";
