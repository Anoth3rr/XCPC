/*
用途：树链剖分版 LCA，适合只需要最近公共祖先、无权距离或 DFS 序的场景。

接口：
- Tree(n)：建立 1-indexed 的 n 点树。
- add(u, v)：加入无向边。
- work(root = 1)：预处理 siz / dep / parent / son / top / dfn / out。
- askLCA(u, v)：返回最近公共祖先。
- askDis(u, v)：返回 u 到 v 的边数距离。
- askAnc(u, v)：判断 u 是否为 v 的祖先（含 u == v）。

dfn 为重链优先 DFS 序，但子树仍是连续区间 [dfn[u], out[u]]，可直接用于虚树。
*/
struct Tree {
    int n, idx;
    vector<vector<int>> ver;
    vector<int> siz, dep, top, son, fa, dfn, out, rev;

    explicit Tree(int n) : n(n), idx(0), ver(n + 1), siz(n + 1), dep(n + 1),
                           top(n + 1), son(n + 1), fa(n + 1), dfn(n + 1),
                           out(n + 1), rev(n + 1) {}

    void add(int u, int v) {
        ver[u].push_back(v);
        ver[v].push_back(u);
    }

    void dfs1(int u) {
        siz[u] = 1;
        for (int v : ver[u]) {
            if (v == fa[u]) continue;
            fa[v] = u;
            dep[v] = dep[u] + 1;
            dfs1(v);
            siz[u] += siz[v];
            if (siz[v] > siz[son[u]]) son[u] = v;
        }
    }

    void dfs2(int u, int t) {
        top[u] = t;
        dfn[u] = ++idx;
        rev[idx] = u;
        if (son[u]) dfs2(son[u], t);
        for (int v : ver[u]) {
            if (v == fa[u] || v == son[u]) continue;
            dfs2(v, v);
        }
        out[u] = idx;
    }

    int askLCA(int u, int v) const {
        while (top[u] != top[v]) {
            if (dep[top[u]] > dep[top[v]]) {
                u = fa[top[u]];
            } else {
                v = fa[top[v]];
            }
        }
        return dep[u] < dep[v] ? u : v;
    }

    int askDis(int u, int v) const {
        return dep[u] + dep[v] - 2 * dep[askLCA(u, v)];
    }

    bool askAnc(int u, int v) const {
        return dfn[u] <= dfn[v] && dfn[v] <= out[u];
    }

    void work(int rt = 1) {
        idx = 0;
        fill(siz.begin(), siz.end(), 0);
        fill(dep.begin(), dep.end(), 0);
        fill(top.begin(), top.end(), 0);
        fill(son.begin(), son.end(), 0);
        fill(fa.begin(), fa.end(), 0);
        fill(dfn.begin(), dfn.end(), 0);
        fill(out.begin(), out.end(), 0);
        fill(rev.begin(), rev.end(), 0);
        dfs1(rt);
        dfs2(rt, rt);
    }
};
