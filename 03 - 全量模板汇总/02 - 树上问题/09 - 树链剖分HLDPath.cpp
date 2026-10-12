struct HLDPath {
    int n, cur = 0;
    vector<vector<int>> g;
    vector<int> fa, dep, siz, son, top, in, out;

    HLDPath(int n) : n(n), g(n + 1), fa(n + 1), dep(n + 1),
        siz(n + 1), son(n + 1), top(n + 1), in(n + 1), out(n + 1) {}

    void addEdge(int x, int y) {
        g[x].push_back(y);
        g[y].push_back(x);
    }
    void build(int root = 1) {
        cur = 0;
        dep[root] = 0;
        auto dfs1 = [&](auto &&self, int x, int p) -> void {
            fa[x] = p, siz[x] = 1, son[x] = 0;
            for (int y : g[x]) {
                if (y == p) continue;
                dep[y] = dep[x] + 1;
                self(self, y, x);
                siz[x] += siz[y];
                if (!son[x] || siz[y] > siz[son[x]]) son[x] = y;
            }
        };
        dfs1(dfs1, root, 0);
        auto dfs2 = [&](auto &&self, int x, int h) -> void {
            top[x] = h, in[x] = ++cur;
            if (son[x]) self(self, son[x], h);
            for (int y : g[x]) {
                if (y != fa[x] && y != son[x]) self(self, y, y);
            }
            out[x] = cur;
        };
        dfs2(dfs2, root, root);
    }
    int lca(int x, int y) const {
        while (top[x] != top[y]) {
            if (dep[top[x]] < dep[top[y]]) swap(x, y);
            x = fa[top[x]];
        }
        return dep[x] < dep[y] ? x : y;
    }
    pair<int, int> subtree(int x) const {
        return {in[x], out[x]};
    }

    template<class F> void splitPath(int x, int y, F &f, bool edge) const {
        while (top[x] != top[y]) {
            if (dep[top[x]] < dep[top[y]]) swap(x, y);
            f(in[top[x]], in[x]);
            x = fa[top[x]];
        }
        if (dep[x] > dep[y]) swap(x, y);
        int l = in[x] + (edge ? 1 : 0);
        if (l <= in[y]) f(l, in[y]);
    }
    template<class F> void forPath(int x, int y, F &&f) const { // 点权路径
        splitPath(x, y, f, false);
    }
    template<class F> void forEdgePath(int x, int y, F &&f) const { // 边权路径
        splitPath(x, y, f, true);
    }
};
