function<void(int, int)> dfs = [&](int u, int fa) {
    for (int v : g[u]) {
        if (v == fa) continue;
        dfs(v, u);
    }
};
dfs(1, 0);
