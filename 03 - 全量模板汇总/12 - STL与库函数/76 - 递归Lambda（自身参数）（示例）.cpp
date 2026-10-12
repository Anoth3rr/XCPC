auto dfs = [&](auto&& self, int u, int fa) -> void {
    for (int v : g[u]) {
        if (v == fa) continue;
        self(self, v, u);
    }
};
dfs(dfs, 1, 0);
