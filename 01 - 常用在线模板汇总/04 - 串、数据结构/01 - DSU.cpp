struct DSU {
    vector<int> f, siz;
    int cc;

    DSU(int n) : f(n + 1), siz(n + 1, 1), cc(n) {
        iota(f.begin(), f.end(), 0LL);
    }

    int find(int x) {
        while (x != f[x]) {
            x = f[x] = f[f[x]];
        }
        return x;
    }

    bool merge(int x, int y) {
        x = find(x), y = find(y);
        if (x == y) return false;
        if (siz[x] < siz[y]) swap(x, y);
        f[y] = x;
        siz[x] += siz[y];
        --cc;
        return true;
    }

    bool same(int x, int y) {
        return find(x) == find(y);
    }

    int getsiz(int x) const {
        return siz[find(x)];
    }

    int getcnt() const {
        return cc;
    }
}