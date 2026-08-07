/*
用途：从若干关键点及其必要 LCA 构造虚树，压缩无关节点。

接口：
- build(points, lca, dfn)：
  * points 可重复；
  * lca(u, v) 可传入原树的 askLCA；
  * dfn(u) 返回原树的先序 DFS 编号（祖先必须在子孙之前）。
- vertex：虚树节点（原树编号，按 dfn 排序）。
- edge：有向边 parent -> child（均为原树节点编号）。
- root：虚树根；空输入时为 0。

使用 03A 或 03B 的 askLCA + dfn/tin 即可。复杂度 O(k log k)（含 LCA 查询）。
*/
struct VTree {
    int rt = 0;
    vector<int> u;
    vector<pair<int, int>> e;

    template <class Lca, class Dfn>
    void build(vector<int> p, Lca lca, Dfn dfn) {
        u.clear();
        e.clear();
        rt = 0;
        if (p.empty()) return;

        auto cmp = [&](int u, int v) { return dfn(u) < dfn(v); };
        sort(p.begin(), p.end(), cmp);
        p.erase(unique(p.begin(), p.end()), p.end());

        int n0 = p.size();
        for (int i = 1; i < n0; ++i) p.push_back(lca(p[i - 1], p[i]));
        sort(p.begin(), p.end(), cmp);
        p.erase(unique(p.begin(), p.end()), p.end());
        u = p;

        vector<int> st;
        for (int u : u) {
            while (!st.empty() && lca(st.back(), u) != st.back()) st.pop_back();
            if (!st.empty()) e.push_back({st.back(), u});
            st.push_back(u);
        }
        rt = u.front();
    }

    vector<vector<int>> buildG(int n) const {
        vector<vector<int>> g(n + 1);
        for (auto [u, v] : e) g[u].push_back(v);
        return g;
    }
};
