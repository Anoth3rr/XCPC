int ans = 0;
vector<int> vis(n + 1);
auto dfs = [&](auto self, int x) -> void {
    vis[x] = 1;
    for (auto y : ver[x]) {
        if (vis[y] == 0) {
            self(self, y);
        } else if (vis[y] == 1) {
            ans++;
        }
    }
    vis[x] = 2;
};
for (int i = 1; i <= n; i++) {
    if (!vis[i]) {
        dfs(dfs, i);
    }
}
cout << ans << endl;
