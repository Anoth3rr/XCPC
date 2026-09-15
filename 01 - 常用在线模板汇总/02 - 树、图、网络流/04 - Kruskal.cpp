struct Tree {
    int n;
    vector<array<int, 3>> e;

    Tree(int n) : n(n);

    void add(int u, int v) {
        e.push_back({w, u, v});
    }

    int kruskal() {
        DSU dsu(n);
        int ans = 0;
        sort(e.begin(), e.end());
        for (auto [w, x, y] : e) {
            if (dsu.merge(x, y)) ans += w, cnt++;
        }
        return ans;
    }
}