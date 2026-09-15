struct Tree {
    int n, lg, tim;
    vector<vector<int>> g, up;
    vector<int> dep, tin, tout;

    Tree(int n) : n(n), tim(0), g(n + 1), dep(n + 1), tin(n + 1), tout(n + 1) {
        lg = 1;
        while ((1LL << lg) <= n) ++lg;
        up.assign(n + 1, vector<int>(lg));
    }

    void add(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs(int u, int fa) {
        up[u][0] = fa;
        tin[u] = ++tim;

        for (auto v : g[u]) {
            if (v == fa) continue;
            dep[v] = dep[u] + 1;
            dfs(v, u);
        }

        tout[u] = tim;
    }

    void work(int root = 1) {
        tim = 0;
        dep[root] = 0;
        dfs(root, 0);

        for (int k = 1; k < lg; ++k) {
            for (int u = 1; u <= n; ++u) {
                up[u][k] = up[up[u][k - 1]][k - 1];
            }
        }
    }

    int jmp(int u, int k) const {
        if (k < 0 || k > dep[u]) return 0;
        for (int i = 0; i < lg; ++i) {
            if ((k >> i) & 1) u = up[u][i];
        }
        return u;
    }

    int lca(int x, int y) const {
        if (dep[x] < dep[y]) swap(x, y);
        x = jmp(x, dep[x] - dep[y]);
        if (x == y) return x;

        for (int k = lg - 1; k >= 0; --k) {
            if (up[x][k] == up[y][k]) continue;
            x = up[x][k], y = up[y][k];
        }
        return up[x][0];
    }

    int dis(int x, int y) const {
        int p = lca(x, y);
        return dep[x] + dep[y] - 2 * dep[p];
    }

    int kth(int x, int y, int k) const {
        int p = lca(x, y);
        int a = dep[x] - dep[p], b = dep[y] - dep[p];
        if (k < 0 || k > a + b) return 0;
        return k <= a ? jmp(x, k) : jmp(y, a + b - k);
    }

    bool anc(int x, int y) const {
        return tin[x] <= tin[y] && tout[y] <= tout[x];
    }
};