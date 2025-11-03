struct DSU {
    vector<int> f, siz;
    vector<array<int, 2>> his;

    DSU() {}
    DSU(int n) {
        init(n);
    }

    void init(int n) {
        f.resize(n + 1);
        iota(f.begin(), f.end(), 0);
        siz.assign(n + 1, 1);
    }

    int find(int x) {
        while (f[x] != x) {
            x = f[x];
        }
        return x;
    }

    void merge(int x, int y) {
        x = find(x), y = find(y);
        if (x == y) return;
        if (siz[x] < siz[y]) swap(x, y);
        his.push_back({x, y});
        f[y] = x;
        siz[x] += siz[y];
    }

    int time() {
        return his.size();
    }

    bool same(int x, int y) {
        return find(x) == find(y);
    }

    int size(int x) {
        return siz[find(x)];
    }

    void revert(int tm) {
        while (his.size() > tm) {
            auto [x, y] = his.back();
            his.pop_back();
            f[y] = y;
            siz[x] -= siz[y];
        }
    }
};