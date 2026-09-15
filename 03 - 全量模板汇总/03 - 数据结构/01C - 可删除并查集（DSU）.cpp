struct DSU {
    vector<int> fa, sz, id;
    int tot = 0;

    DSU() = default;
    explicit DSU(int n) {
        init(n);
    }

    void init(int n) {
        tot = n;
        fa.resize(n + 1);
        iota(all(fa), 0);
        sz.assign(n + 1, 1);
        id.resize(n + 1);
        for (int i = 1; i <= n; ++i) id[i] = i;
    }

    int find(int x) {
        return fa[x] == x ? x : fa[x] = find(fa[x]);
    }

    int node() {
        ++tot;
        fa.push_back(tot);
        sz.push_back(1);
        return tot;
    }

    bool merge(int x, int y) {
        int a = find(id[x]);
        int b = find(id[y]);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        fa[b] = a;
        sz[a] += sz[b];
        return true;
    }

    bool link(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        fa[b] = a;
        sz[a] += sz[b];
        return true;
    }

    void erase(int x) {
        --sz[find(id[x])];
        id[x] = node();
    }

    bool ask(int x, int y) {
        return find(id[x]) == find(id[y]);
    }

    int askSz(int x) {
        return sz[find(id[x])];
    }
};
