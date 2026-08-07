/*
用途：树链剖分，把路径/子树转为 DFS 序区间。
约定：节点、DFS 序和重儿子数组使用 int；区间回调仍可接收默认 int。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
struct HLD {

    int n, tm = 0;
    vector<vector<int>> g;
    vector<int> fa, dep, sz, son, top, in, out, rid;

    explicit HLD(int n)
        : n(n), g(n + 1), fa(n + 1), dep(n + 1), sz(n + 1), son(n + 1), top(n + 1), in(n + 1),
          out(n + 1), rid(n + 1) {
        assert(n >= 0);
    }

    void add(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs1(int u, int f) {
        fa[u] = f;
        sz[u] = 1;
        for (int v : g[u]) {
            if (v == f) continue;
            dep[v] = dep[u] + 1;
            dfs1(v, u);
            sz[u] += sz[v];
            if (sz[v] > sz[son[u]]) son[u] = v;
        }
    }

    void dfs2(int u, int h) {
        top[u] = h;
        in[u] = ++tm;
        rid[tm] = u;
        if (son[u]) dfs2(son[u], h);
        for (int v : g[u]) {
            if (v == fa[u] || v == son[u]) continue;
            dfs2(v, v);
        }
        out[u] = tm;
    }

    void work(int rt = 1) {
        assert(1 <= rt && rt <= n);
        tm = 0;
        fill(fa.begin(), fa.end(), 0);
        fill(dep.begin(), dep.end(), 0);
        fill(sz.begin(), sz.end(), 0);
        fill(son.begin(), son.end(), 0);
        fill(top.begin(), top.end(), 0);
        fill(in.begin(), in.end(), 0);
        fill(out.begin(), out.end(), 0);
        fill(rid.begin(), rid.end(), 0);
        dfs1(rt, 0);
        dfs2(rt, rt);
    }

    int askLCA(int u, int v) const {
        while (top[u] != top[v]) {
            if (dep[top[u]] > dep[top[v]]) u = fa[top[u]];
            else v = fa[top[v]];
        }
        return dep[u] < dep[v] ? u : v;
    }

    int askDis(int u, int v) const {
        int p = askLCA(u, v);
        return dep[u] + dep[v] - 2 * dep[p];
    }

    pair<int, int> askSub(int u) const {
        return {in[u], out[u]};
    }

    template <class F> void pathV(int u, int v, F&& f) const {
        while (top[u] != top[v]) {
            if (dep[top[u]] < dep[top[v]]) swap(u, v);
            f(in[top[u]], in[u]);
            u = fa[top[u]];
        }
        if (dep[u] > dep[v]) swap(u, v);
        f(in[u], in[v]);
    }

    template <class F> void pathE(int u, int v, F&& f) const {
        while (top[u] != top[v]) {
            if (dep[top[u]] < dep[top[v]]) swap(u, v);
            f(in[top[u]], in[u]);
            u = fa[top[u]];
        }
        if (dep[u] > dep[v]) swap(u, v);
        if (u != v) f(in[u] + 1, in[v]);
    }
};
