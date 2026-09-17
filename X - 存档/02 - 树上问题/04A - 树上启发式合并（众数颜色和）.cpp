struct DSUOnTree {
    vector<vector<int>> e;
    vector<int> siz, son, cnt, col;
    vector<int> ans;
    int sum, mx;
    int hs;
    DSUOnTree(int n, const vector<int> &c = {}) {
        e.resize(n + 1);
        siz.resize(n + 1);
        son.resize(n + 1);
        ans.resize(n + 1);
        cnt.resize(n + 1);
        col.assign(n + 1, 0);
        if (!c.empty()) {
            assert(c.size() == n + 1);
            col = c;
            for (int i = 1; i <= n; ++i) assert(0 <= col[i] && col[i] <= n);
        }
        hs = 0;
        sum = 0;
        mx = 0;
    }
    void setCol(const vector<int> &c) {
        assert(c.size() == col.size());
        col = c;
        for (int i = 1; i < col.size(); ++i) assert(0 <= col[i] && col[i] < cnt.size());
    }
    void add(int u, int v) {
        e[u].push_back(v);
        e[v].push_back(u);
    }
    void dfs1(int u, int fa) {
        siz[u] = 1;
        for (auto v : e[u]) {
            if (v == fa) continue;
            dfs1(v, u);
            siz[u] += siz[v];
            if (siz[v] > siz[son[u]]) son[u] = v;
        }
    }
    void calc(int u, int fa, int val) {
        cnt[col[u]] += val;
        if (cnt[col[u]] > mx) {
            mx = cnt[col[u]];
            sum = col[u];
        } else if (cnt[col[u]] == mx) {
            sum += col[u];
        }
        for (auto v : e[u]) {
            if (v == fa || v == hs) continue;
            calc(v, u, val);
        }
    }
    void dfs2(int u, int fa, int opt) {
        for (auto v : e[u]) {
            if (v == fa || v == son[u]) continue;
            dfs2(v, u, 0);
        }
        if (son[u]) {
            dfs2(son[u], u, 1);
            hs = son[u];
        }
        calc(u, fa, 1);
        hs = 0;
        ans[u] = sum;
        if (!opt) {
            calc(u, fa, -1);
            sum = 0;
            mx = 0;
        }
    }
    void work(int rt = 1) {
        dfs1(rt, 0);
        dfs2(rt, 0, 0);
    }
};
