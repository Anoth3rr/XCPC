struct Tree {
    int n;
    vector<vector<int>> ver;
    vector<vector<pii>> qry;
    vector<int> fa, ans;
    vector<char> vis;

    Tree(int n) : n(n), ver(n + 1), qry(n + 1), fa(n + 1), vis(n + 1) {}

    void add(int u, int v) {
        ver[u].push_back(v);
        ver[v].push_back(u);
    }

    int ask(int u, int v) {
        int id = ans.size();
        ans.push_back(0);
        qry[u].push_back({v, id});
        if (u != v) qry[v].push_back({u, id});
        return id;
    }

    int find(int u) {
        return fa[u] == u ? u : fa[u] = find(fa[u]);
    }

    void dfs(int u, int f) {
        fa[u] = u;
        for (int v : ver[u]) {
            if (v == f) continue;
            dfs(v, u);
            fa[find(v)] = u;
        }
        vis[u] = true;
        for (auto [v, id] : qry[u]) {
            if (vis[v]) ans[id] = find(v);
        }
    }

    vector<int> work(int rt = 1) {
        iota(fa.begin(), fa.end(), 0);
        fill(vis.begin(), vis.end(), false);
        fill(ans.begin(), ans.end(), 0);
        dfs(rt, 0);
        return ans;
    }
};
