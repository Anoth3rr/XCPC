struct Tree {
    int n, lg;
    vector<vector<int>> ver, val, up, dn;
    vector<int> dep, fa, son, len, top;

    Tree(int n) : n(n), ver(n + 1), dep(n + 1), fa(n + 1), son(n + 1), len(n + 1), top(n + 1) {
        lg = 1;
        while ((1LL << lg) <= max<int>(1, n)) ++lg;
        val.assign(n + 1, vector<int>(lg));
    }

    void add(int u, int v) {
        ver[u].push_back(v);
        ver[v].push_back(u);
    }

    void dfs1(int u, int f) {
        fa[u] = f;
        val[u][0] = f;
        for (int k = 1; k < lg; ++k) val[u][k] = val[val[u][k - 1]][k - 1];
        len[u] = 1;
        for (int v : ver[u]) {
            if (v == f) continue;
            dep[v] = dep[u] + 1;
            dfs1(v, u);
            if (len[v] > len[son[u]]) son[u] = v;
        }
        len[u] = len[son[u]] + 1;
    }

    void dfs2(int u, int t) {
        top[u] = t;
        if (u == t) {
            up[u].assign(len[u], 0);
            dn[u].assign(len[u], 0);
            up[u][0] = u;
            for (int i = 1; i < len[u]; ++i) up[u][i] = fa[up[u][i - 1]];
        }
        dn[t][dep[u] - dep[t]] = u;
        if (son[u]) dfs2(son[u], t);
        for (int v : ver[u]) {
            if (v == fa[u] || v == son[u]) continue;
            dfs2(v, v);
        }
    }

    int askJmp(int u, int k) const {
        if (k < 0 || k > dep[u]) return 0;
        if (!k) return u;
        int b = 63 - __builtin_clzll(k);
        u = val[u][b];
        k -= 1LL << b;
        if (!k) return u;
        int t = top[u], d = dep[u] - dep[t];
        return k <= d ? dn[t][d - k] : up[t][k - d];
    }

    int askLCA(int u, int v) const {
        if (dep[u] < dep[v]) swap(u, v);
        u = askJmp(u, dep[u] - dep[v]);
        if (u == v) return u;
        for (int k = lg - 1; k >= 0; --k) {
            if (val[u][k] == val[v][k]) continue;
            u = val[u][k];
            v = val[v][k];
        }
        return val[u][0];
    }

    int askDis(int u, int v) const {
        return dep[u] + dep[v] - 2 * dep[askLCA(u, v)];
    }

    void work(int rt = 1) {
        fill(dep.begin(), dep.end(), 0);
        fill(fa.begin(), fa.end(), 0);
        fill(son.begin(), son.end(), 0);
        fill(len.begin(), len.end(), 0);
        fill(top.begin(), top.end(), 0);
        for (auto &x : val) fill(x.begin(), x.end(), 0);
        up.assign(n + 1, {});
        dn.assign(n + 1, {});
        dfs1(rt, 0);
        dfs2(rt, rt);
    }
};
