/*
用途：无权树直径模板。两次 DFS/遍历即可求直径长度。

接口：
- Tree(n)：建立 1-indexed 的 n 点树。
- add(u, v)：加入无向边。
- askEnds(root = 1)：返回一组直径端点。
- askLen(root = 1)：返回直径长度（边数）。
*/
struct Tree {
    int n;
    vector<vector<int>> ver;

    explicit Tree(int n) : n(n), ver(n + 1) {}

    void add(int u, int v) {
        ver[u].push_back(v);
        ver[v].push_back(u);
    }

    pair<int, vector<int>> far(int src) const {
        vector<int> dep(n + 1, -1), st = {src};
        dep[src] = 0;
        for (int i = 0; i < st.size(); ++i) {
            int u = st[i];
            for (int v : ver[u]) {
                if (dep[v] != -1) continue;
                dep[v] = dep[u] + 1;
                st.push_back(v);
            }
        }

        int far = src;
        for (int u = 1; u <= n; ++u) {
            if (dep[u] > dep[far]) far = u;
        }
        return {far, dep};
    }

    pair<int, int> askEnds(int rt = 1) const {
        int u = far(rt).first;
        int v = far(u).first;
        return {u, v};
    }

    int askLen(int rt = 1) const {
        int u = far(rt).first;
        auto [v, dep] = far(u);
        return dep[v];
    }
};
