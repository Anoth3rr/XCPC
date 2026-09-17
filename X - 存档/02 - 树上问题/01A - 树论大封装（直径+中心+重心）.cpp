struct Tree {
    int n;
    vector<vector<pii>> g;
    vector<int> dep, fa, mxd, d1, d2, up, s1, s2;
    Tree(int n) : n(n), g(n + 1), dep(n + 1), fa(n + 1), mxd(n + 1), d1(n + 1), d2(n + 1), up(n + 1), s1(n + 1), s2(n + 1) {}

    void add(int u, int v, int w = 1) {
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }

    void dfs(int u, int _fa) {
        mxd[u] = dep[u];
        for (auto [v, w] : g[u]) {
            if (v == _fa) continue;
            dep[v] = dep[u] + 1;
            fa[v] = u;
            dfs(v, u);
            mxd[u] = max(mxd[u], mxd[v]);
        }
    }

    void dfs1(int u, int _fa) {
        for (auto [v, w] : g[u]) {
            if (v == _fa) continue;
            dfs1(v, u);
            int cur = d1[v] + w;
            if (cur > d1[u]) {
                d2[u] = d1[u], s2[u] = s1[u];
                d1[u] = cur, s1[u] = v;
            } else if (cur > d2[u]) {
                d2[u] = cur, s2[u] = v;
            }
        }
    }

    void dfs2(int u, int _fa) {
        for (auto [v, w] : g[u]) {
            if (v == _fa) continue;
            up[v] = (s1[u] == v ? max(up[u], d2[u]) : max(up[u], d1[u])) + w;
            dfs2(v, u);
        }
    }

    int rad, dia, cen;
    void getCen() {
        cen = 1; // 中心
        for (int u = 1; u <= n; ++u) {
            if (max(d1[u], up[u]) < max(d1[cen], up[cen])) cen = u;
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
        rem = numeric_limits<int>::max();
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

    pair<int, vector<int>> far(int s) const {
        vector<int> _fa(n + 1, -1), st = {s};
        vector<int> dis(n + 1);
        _fa[s] = 0;
        for (int i = 0; i < st.size(); ++i) {
            int u = st[i];
            for (auto [v, w] : g[u]) {
                if (v == _fa[u]) continue;
                _fa[v] = u;
                dis[v] = dis[u] + w;
                st.push_back(v);
            }
        }
        int f = s;
        for (int u = 1; u <= n; ++u) {
            if (dis[u] > dis[f]) f = u;
        }
        return {f, dis};
    }

    pair<int, int> askEnds() {
        auto [u, ban] = far(1);
        auto [v, dis] = far(u);
        dia = dis[v];
        return {u, v};
    }

    void init(int n) {
        this->n = n;
        g.resize(n + 1);
        dep.resize(n + 1);
        fa.resize(n + 1);
        mxd.resize(n + 1);
        d1.resize(n + 1);
        d2.resize(n + 1);
        s1.resize(n + 1);
        s2.resize(n + 1);
        up.resize(n + 1);
    }

    void work(int rt = 1) {
        dfs(rt, 0);
        dfs1(rt, 0);
        dfs2(rt, 0);
        getCen();
        getCog();
    }
};