template <class T> struct DSU {
    vector<int> fa, sz;
    vector<T> w;

    DSU() = default;
    explicit DSU(int n) {
        init(n);
    }

    void init(int n) {
        fa.resize(n + 1);
        iota(all(fa), 0);
        sz.assign(n + 1, 1);
        w.assign(n + 1, T{});
    }

    int find(int x) {
        if (fa[x] == x) return x;
        int p = fa[x];
        int r = find(p);
        w[x] = w[x] + w[p];
        return fa[x] = r;
    }

    T ask(int x) {
        find(x);
        return w[x];
    }

    bool merge(int x, int y, const T &d) {
        int fx = find(x), fy = find(y);
        if (fx == fy) return w[x] - w[y] == d;

        if (sz[fx] < sz[fy]) {
            fa[fx] = fy;
            w[fx] = d - w[x] + w[y];
            sz[fy] += sz[fx];
        } else {
            fa[fy] = fx;
            w[fy] = w[x] - w[y] - d;
            sz[fx] += sz[fy];
        }
        return true;
    }

    bool ask(int x, int y) {
        return find(x) == find(y);
    }
};
