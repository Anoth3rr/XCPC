struct XorDSU {
    vector<int> fa, sz, w;

    XorDSU() = default;
    explicit XorDSU(int n) {
        init(n);
    }

    void init(int n) {
        fa.resize(n);
        iota(fa.begin(), fa.end(), 0);
        sz.assign(n, 1);
        w.assign(n, 0);
    }

    int find(int x) {
        if (fa[x] == x) return x;
        int p = fa[x];
        fa[x] = find(p);
        w[x] ^= w[p];
        return fa[x];
    }

    int ask(int x) {
        find(x);
        return w[x];
    }

    bool merge(int x, int y, int d) {
        int fx = find(x), fy = find(y);
        if (fx == fy) return (w[x] ^ w[y]) == d;
        if (sz[fx] < sz[fy]) swap(fx, fy);
        fa[fy] = fx;
        w[fy] = w[x] ^ w[y] ^ d;
        sz[fx] += sz[fy];
        return true;
    }
};
