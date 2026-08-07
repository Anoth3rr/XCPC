/*
用途：Gomory-Hu Tree（无向图任意两点最小割）。
依赖：01A - 最大流（Dinic）的 MaxFlow。
约定：点编号 0..n-1，边容量非负；build 后 g 是一棵带权树，
任意两点最小割等于树路径上的最小边权。ask(u,v) 单次 O(log n)。
复杂度：建树调用 n-1 次最大流，额外 O(n log n) 预处理。
*/

template <class Cap = int> struct GHTree {
    static constexpr Cap INF = numeric_limits<Cap>::max() / 4;

    int n;
    MaxFlow<Cap> mf;
    vector<vector<pair<int, Cap>>> g;
    vector<int> dep;
    vector<vector<int>> up;
    vector<vector<Cap>> mn;
    bool ok = false;

    explicit GHTree(int n) : n(n), mf(n), g(n) {
        assert(n > 0);
    }

    void add(int u, int v, Cap c) {
        assert(0 <= u && u < n && 0 <= v && v < n && c >= 0);
        mf.add(u, v, c);
        mf.add(v, u, c);
        ok = false;
    }

    void build() {
        vector<int> fa(n, 0);
        vector<Cap> cut(n);
        fa[0] = -1;

        for (int s = 1; s < n; ++s) {
            int t = fa[s];
            mf.reset();
            Cap val = mf.flow(s, t);
            vector<char> sd = mf.askCut(s);
            cut[s] = val;

            for (int u = s + 1; u < n; ++u) {
                if (fa[u] == t && sd[u]) fa[u] = s;
            }
            if (fa[t] != -1 && sd[fa[t]]) {
                fa[s] = fa[t];
                fa[t] = s;
                cut[s] = cut[t];
                cut[t] = val;
            }
        }

        g.assign(n, {});
        for (int u = 1; u < n; ++u) {
            g[u].push_back({fa[u], cut[u]});
            g[fa[u]].push_back({u, cut[u]});
        }
        init();
        ok = true;
    }

    Cap ask(int u, int v) const {
        assert(ok && 0 <= u && u < n && 0 <= v && v < n);
        if (u == v) return 0;
        Cap ans = INF;
        if (dep[u] < dep[v]) swap(u, v);
        int d = dep[u] - dep[v];
        for (int bit = 0; d > 0; ++bit, d >>= 1) {
            if (d & 1) {
                ans = min(ans, mn[bit][u]);
                u = up[bit][u];
            }
        }
        if (u == v) return ans;

        for (int bit = up.size(); bit--;) {
            if (up[bit][u] == up[bit][v]) continue;
            ans = min(ans, mn[bit][u]);
            ans = min(ans, mn[bit][v]);
            u = up[bit][u];
            v = up[bit][v];
        }
        ans = min(ans, mn[0][u]);
        ans = min(ans, mn[0][v]);
        return ans;
    }

  private:
    void init() {
        int log = 1;
        while ((1 << log) <= n) ++log;
        dep.assign(n, 0);
        up.assign(log, vector<int>(n));
        mn.assign(log, vector<Cap>(n, INF));

        queue<int> q;
        vector<char> vis(n, false);
        q.push(0);
        vis[0] = true;
        up[0][0] = 0;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto [v, w] : g[u]) {
                if (vis[v]) continue;
                vis[v] = true;
                dep[v] = dep[u] + 1;
                up[0][v] = u;
                mn[0][v] = w;
                q.push(v);
            }
        }

        for (int bit = 1; bit < log; ++bit) {
            for (int u = 0; u < n; ++u) {
                up[bit][u] = up[bit - 1][up[bit - 1][u]];
                mn[bit][u] = min(mn[bit - 1][u], mn[bit - 1][up[bit - 1][u]]);
            }
        }
    }
};
