struct Tree {
    int n;
    vector<vector<int>> g;
    Tree(int n) : n(n), g(n + 1) {}
    void add(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }
    array<int, 3> getlen(int root = 1) const {
        vector<int> dep(n + 1, -1);
        auto dfs = [&](int s, int fa) {
            vector<pii> st = {{s, fa}};
            dep[s] = 0;
            int r = s;
            for (; st.size();) {
                auto [u, fa] = st.back();
                st.pop_back();
                if (dep[u] > dep[r]) r = u;
                for (auto v : g[u]) {
                    if (v == fa) continue;
                    dep[v] = dep[u] + 1;
                    st.push_back({v, u});
                }
            }
            return r;
        };
        int st = dfs(root, 0);
        int ed = dfs(st, 0);
        return {dep[ed], st, ed};
    }
};