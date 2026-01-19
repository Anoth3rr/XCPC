template <class Int = ll>
struct DSU {
    vector<int> f, siz;
    vector<Int> w;

    DSU(int n_) {
        init(n_);
    }

    void init(int n) {
        f.resize(n + 1);
        iota(f.begin(), f.end(), 0);
        siz.assign(n + 1, 1);
        w.assign(n + 1, 0);
    }

    int find(int x) {
        if (f[x] == x)
            return x;
        int fx = find(f[x]);
        w[x] += w[f[x]];
        return f[x] = fx;
    }

    Int get(int x) {
        find(x);
        return w[x];
    }

    bool merge(int x, int y, Int diff) {
        int fx = find(x), fy = find(y);
        if (fx == fy) {
            return (w[x] - w[y] == diff);
        }
        if (siz[fx] < siz[fy]) {
            f[fx] = fy;
            w[fx] = diff - w[x] + w[y];
            siz[fy] += siz[fx];
        } else {
            f[fy] = fx;
            w[fy] = w[x] - w[y] - diff;
            siz[fx] += siz[fy];
        }
        return true;
    }

    bool same(int x, int y) {
        return find(x) == find(y);
    }
};
