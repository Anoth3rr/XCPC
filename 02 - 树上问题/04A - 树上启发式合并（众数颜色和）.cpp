/*
用途：树上启发式合并（DSU on Tree），当前模板用于统计每个子树中出现次数最多的颜色编号之和。
依赖：
- 外部数组 color[u] 表示点 u 的颜色；若颜色值可能大于 n，需要先离散化或扩大 cnt。
接口：
- DSUOnTree(n)：建立 1-indexed 的 n 点树辅助结构。
- add(u, v)：加入无向边。
- dfs1(root, 0)：预处理子树大小和重儿子。
- dfs2(root, 0, 0)：计算 ans[u]。
- work(root = 1)：一次性完成 dfs1 和 dfs2。
- ans[u]：子树 u 中最高出现次数的颜色编号之和。
*/

struct DSUOnTree {
    vector<vector<int>> e;
    vector<int> siz, son, cnt;
    vector<int> ans;
    int sum, mx;
    int hs;
    DSUOnTree(int n) {
        e.resize(n + 1);
        siz.resize(n + 1);
        son.resize(n + 1);
        ans.resize(n + 1);
        cnt.resize(n + 1);
        hs = 0;
        sum = 0;
        mx = 0;
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
