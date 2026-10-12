struct Tree {
    int n;
    vector<vector<pii>> g;
    vector<int> dep, f, mxd, d1, d2, s1, s2, up;
    Tree(int n) : n(n), g(n + 1), dep(n + 1), f(n + 1), mxd(n + 1), d1(n + 1), d2(n + 1), s1(n + 1), s2(n + 1), up(n + 1) {}
    void add(int u, int v, int w) {
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }
    void dfs(int u, int fa) {
        mxd[u] = dep[u];
        for (auto [v, w] : g[u]) {
            if (v == fa) continue;
            dep[v] = dep[u] + 1;
            f[v] = u;
            dfs(v, u);
            mxd[u] = max(mxd[u], mxd[v]);
        }
    }

    void dfs1(int u, int fa) {
        for (auto [v, w] : g[u]) {
            if (v == fa) continue;
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
    void dfs2(int u, int fa) {
        for (auto [v, w] : g[u]) {
            if (v == fa) continue;
            up[v] = (s1[u] == v ? max(up[u], d2[u]) : max(up[u], d1[u])) + w;
            dfs2(v, u);
        }
    }

    int rad, dia, cen;
    void getCenter() {
        cen = 1; // 中心
        for (int i = 1; i <= n; i++) {
            if (max(d1[i], up[i]) < max(d1[cen], up[cen])) cen = i;
        }
        rad = max(d1[cen], up[cen]); // 距离最远点的距离的最小值
        dia = 0;                     // 直径
        for (int u = 1; u <= n; ++u) {
            dia = max(dia, d1[u] + d2[u]);
        }
    }

    int rem, cog;
    vector<bool> vis;
    void getCog() {
        vis.assign(n + 1, 0);
        rem = inf;
        cog = 1;
        dfsCog(1);
    }
    int dfsCog(int u) {
        vis[u] = true;
        int siz = 1, res = 0;
        for (auto [v, w] : g[u]) {
            if (vis[v]) continue;
            int sub = dfsCog(v);
            res = max(res, sub);
            siz += sub;
        }
        res = max(res, n - siz);
        if (res < rem) rem = res, cog = u;
        return siz;
    }
};
