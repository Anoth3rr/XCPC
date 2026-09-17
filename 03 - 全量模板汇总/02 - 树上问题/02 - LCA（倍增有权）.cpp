constexpr int inf = 2e18 + 9;

struct Tree {
    int n, lg, tim;
    vector<vector<int>> up, Max;
    vector<vector<pii>> g;
    vector<int> dep, tin, tout, dist;

    Tree(int n) : n(n), tim(0), g(n + 1), dep(n + 1), tin(n + 1), tout(n + 1), dist(n + 1) {
        lg = 1;
        while ((1LL << lg) <= n) ++lg;
        up.assign(n + 1, vector<int>(lg));
        Max.assign(n + 1, vector<int>(lg, -inf));
    }

    void add(int u, int v, int w) {
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }

    void dfs(int u, int fa) {
        up[u][0] = fa;
        for (int k = 1; k < lg; ++k) {
            up[u][k] = up[up[u][k - 1]][k - 1];
            Max[u][k] = max(Max[u][k - 1], Max[up[u][k - 1]][k - 1]);
        }
        tin[u] = ++tim;
        for (auto [v, w] : g[u]) {
            if (v == fa) continue;
            dep[v] = dep[u] + 1;
            dist[v] = dist[u] + w;
            Max[v][0] = w;
            dfs(v, u);
        }
        tout[u] = tim;
    }

    void work(int root = 1) {
        tim = 0;
        dep[root] = 0;
        dist[root] = 0;
        Max[root][0] = -inf;
        dfs(root, 0);
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
        return dist[x] + dist[y] - 2 * dist[p];
    }

    int getup(int u, int k) const {
        int r = -inf;
        for (int i = 0; i < lg; ++i) {
            if ((k >> i) & 1) {
                r = max(r, Max[u][i]);
                u = up[u][i];
            }
        }
        return r;
    }

    int getmax(int x, int y) const {
        if (x == y) return 0;
        int p = lca(x, y);
        return max(getup(x, dep[x] - dep[p]), getup(y, dep[y] - dep[p]));
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