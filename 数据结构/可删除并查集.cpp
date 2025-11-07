struct DSU {
    vector<int> f, siz, id;
    int tot;

    DSU(int n_) {
        init(n_);
    }
    void init(int n) {
        tot = n;
        f.resize(n + 1);
        iota(f.begin(), f.end(), 0);
        siz.assign(n + 1, 1);
        id.resize(n + 1);
        for (int i = 1; i <= n; ++i) id[i] = i;
    }

    int add() {
        ++tot;
        f.push_back(tot);
        siz.push_back(1);
        return tot;
    }

    int find(int x) {
        return f[x] == x ? x : f[x] = find(f[x]);
    }

    bool merge1(int x, int y) {
        int a = id[x], b = id[y];
        return merge2(a, b);
    }

    bool merge2(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (siz[a] < siz[b]) swap(a, b);
        f[b] = a;
        siz[a] += siz[b];
        return true;
    }

    void remove(int x) {
        int nd = add();
        id[x] = nd;
    }

    bool same(int x, int y) {
        return find(id[x]) == find(id[y]);
    }

    int size(int x) {
        return siz[find(id[x])];
    }
};
