struct VTree {
    int rt = 0;
    vector<int> u;
    vector<pair<int, int>> e;

    template <class Lca, class Dfn> void build(vector<int> p, Lca lca, Dfn dfn) {
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
