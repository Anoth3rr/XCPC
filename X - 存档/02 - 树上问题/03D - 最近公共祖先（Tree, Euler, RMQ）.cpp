struct Tree {
    int n, idx;
    vector<vector<int>> ver;

    vector<int> dep, fa, dfn, out, rev;
    vector<int> first, euler, lg;
    vector<vector<int>> st;

    Tree(int n) : n(n), idx(0), ver(n + 1), dep(n + 1), fa(n + 1), dfn(n + 1), out(n + 1), rev(n + 1), first(n + 1, -1) {}

    void add(int u, int v) {
        ver[u].push_back(v);
        ver[v].push_back(u);
    }

    int better(int x, int y) const {
        return dep[euler[x]] < dep[euler[y]] ? x : y;
    }

    void dfs(int u, int f) {
        fa[u] = f;
        dfn[u] = ++idx;
        rev[idx] = u;

        first[u] = euler.size();
        euler.push_back(u);

        for (int v : ver[u]) {
            if (v == f) continue;
            dep[v] = dep[u] + 1;
            dfs(v, u);
            euler.push_back(u);
        }

        out[u] = idx;
    }

    void work(int rt = 1) {
        idx = 0;
        fill(dep.begin(), dep.end(), 0);
        fill(fa.begin(), fa.end(), 0);
        fill(dfn.begin(), dfn.end(), 0);
        fill(out.begin(), out.end(), 0);
        fill(rev.begin(), rev.end(), 0);
        fill(first.begin(), first.end(), -1);

        euler.clear();
        euler.reserve(2 * n - 1);
        dfs(rt, 0);

        int m = euler.size();
        lg.assign(m + 1, 0);
        for (int i = 2; i <= m; ++i) lg[i] = lg[i >> 1] + 1;

        int K = lg[m] + 1;
        st.assign(K, vector<int>(m));
        iota(st[0].begin(), st[0].end(), 0);

        for (int k = 1; k < K; ++k) {
            for (int i = 0; i + (1 << k) <= m; ++i) {
                st[k][i] = better(st[k - 1][i], st[k - 1][i + (1 << (k - 1))]);
            }
        }
    }

    int askLCA(int u, int v) const {
        int l = first[u], r = first[v];
        if (l > r) swap(l, r);

        int k = lg[r - l + 1];
        int x = st[k][l];
        int y = st[k][r - (1 << k) + 1];
        return euler[better(x, y)];
    }

    int askDis(int u, int v) const {
        return dep[u] + dep[v] - 2 * dep[askLCA(u, v)];
    }

    bool askAnc(int u, int v) const {
        return dfn[u] <= dfn[v] && dfn[v] <= out[u];
    }
};
