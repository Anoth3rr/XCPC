struct Tree {
    int n;
    vector<vector<pii>> g;
    vector<int> dep, fa, maxdep, d1, d2, s1, s2, up;
    Tree(int n) : n(n), g(n + 1), dep(n + 1), fa(n + 1), maxdep(n + 1), d1(n + 1), d2(n + 1), s1(n + 1), s2(n + 1), up(n + 1) {}
    void add(int u, int v, int w) {
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }
    void dfs(int u, int f) {
        maxdep[u] = dep[u];
        for (auto [v, w] : g[u]) {
            if (v == f) continue;
            dep[v] = dep[u] + 1;
            fa[v] = u;
            dfs(v, u);
            maxdep[u] = max(maxdep[u], maxdep[v]);
        }
    }
    void dfs1(int u, int f) {
        for (auto [v, w] : g[u]) {
            if (v == f) continue;
            dfs1(v, u);
            int x = d1[v] + w;
            if (x > d1[u]) {
                d2[u] = d1[u], s2[u] = s1[u];
                d1[u] = x, s1[u] = v;
            } else if (x > d2[u]) {
                d2[u] = x, s2[u] = v;
            }
        }
    }
    void dfs2(int u, int f) {
        for (auto [v, w] : g[u]) {
            if (v == f) continue;
            if (s1[u] == v) {
                up[v] = max(up[u], d2[u]) + w;
            } else {
                up[v] = max(up[u], d1[u]) + w;
            }
            dfs2(v, u);
        }
    }

    int radius, center, diam;
    void getCenter() {
        center = 1; // 中心
        for (int i = 1; i <= n; i++) {
            if (max(d1[i], up[i]) < max(d1[center], up[center])) {
                center = i;
            }
        }
        radius = max(d1[center], up[center]); // 距离最远点的距离的最小值
        diam = d1[center] + up[center] + 1;   // 直径
    }

    int rem; // 删除重心后剩余连通块体积的最小值
    int cog; // 重心
    vector<bool> vis;
    void getCog() {
        vis.resize(n);
        rem = INT_MAX;
        cog = 1;
        dfsCog(1);
    }
    int dfsCog(int u) {
        vis[u] = true;
        int s = 1, res = 0;
        for (auto [v, w] : g[u]) {
            if (vis[v]) continue;
            int t = dfsCog(v);
            res = max(res, t);
            s += t;
        }
        res = max(res, n - s);
        if (res < rem) {
            rem = res;
            cog = u;
        }
        return s;
    }
};