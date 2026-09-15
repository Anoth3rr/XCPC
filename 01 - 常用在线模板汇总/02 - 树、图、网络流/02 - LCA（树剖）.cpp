struct Tree {
    int n, tim;
    vector<vector<int>> g;
    vector<int> fa, dep, siz, son, top, dfn, out, rev;

    Tree(int n) : n(n), tim(0), g(n + 1), fa(n + 1), dep(n + 1), siz(n + 1), son(n + 1), top(n + 1), dfn(n + 1), out(n + 1), rev(n + 1) {}

    void add(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs1(int u, int p) {
        fa[u] = p, siz[u] = 1, son[u] = 0;

        for (auto v : g[u]) {
            if (v == p) continue;
            dep[v] = dep[u] + 1;
            dfs1(v, u);
            siz[u] += siz[v];
            if (siz[v] > siz[son[u]]) son[u] = v;
        }
    }

    void dfs2(int u, int tp) {
        top[u] = tp;
        dfn[u] = ++tim;
        rev[tim] = u;

        if (son[u]) dfs2(son[u], tp);
        for (auto v : g[u]) {
            if (v == fa[u] || v == son[u]) continue;
            dfs2(v, v);
        }

        out[u] = tim;
    }

    void work(int root = 1) {
        tim = 0;
        dep[root] = 0;
        dfs1(root, 0);
        dfs2(root, root);
    }

    int lca(int u, int v) const {
        while (top[u] != top[v]) {
            if (dep[top[u]] < dep[top[v]]) swap(u, v);
            u = fa[top[u]];
        }
        return dep[u] < dep[v] ? u : v;
    }

    int dis(int u, int v) const {
        int p = lca(u, v);
        return dep[u] + dep[v] - 2 * dep[p];
    }

    bool anc(int u, int v) const {
        return dfn[u] <= dfn[v] && dfn[v] <= out[u];
    }

    int jmp(int u, int k) const {
        if (k < 0 || k > dep[u]) return 0;

        while (k) {
            int h = top[u];
            int len = dep[u] - dep[h];

            if (k <= len) return rev[dfn[u] - k];

            k -= len + 1;
            u = fa[h];
        }
        return u;
    }

    int kth(int u, int v, int k) const {
        int p = lca(u, v);
        int a = dep[u] - dep[p], b = dep[v] - dep[p];

        if (k < 0 || k > a + b) return 0;
        return k <= a ? jmp(u, k) : jmp(v, a + b - k);
    }

    pii subtree(int u) const {
        return {dfn[u], out[u]};
    }

    template <class F> void path(int u, int v, F f) const {
        while (top[u] != top[v]) {
            if (dep[top[u]] < dep[top[v]]) swap(u, v);
            f(dfn[top[u]], dfn[u]);
            u = fa[top[u]];
        }
        if (dep[u] > dep[v]) swap(u, v);
        f(dfn[u], dfn[v]);
    }
};
