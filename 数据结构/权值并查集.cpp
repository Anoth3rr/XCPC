template<class Int>
struct DSU {
    vector<int> f, siz;
    vector<Int> w;

    DSU(int n_) {
        init(n);
    }

    void init(int n) {
        f.resize(n + 1);
        iota(f.begin(), f.end(), 0);
        siz.assign(n + 1, 1);
        w.assign(n + 1, 0);
    }

    int find(int x) {
        if (f[x] == x) return x;
        int fx = find(f[x]);
        w[x] += w[f[x]];
        return f[x] = fx;
    }

    bool merge(int x, int y, int diff) {
        int fx = find(x), fy = find(y);
        if (fx == fy) {
            return (w[y] - w[x] == diff);
        }
        if (siz[fx] < siz[fy]) {
            swap(x, y);
            swap(fx, fy);
            diff = -diff;
        }
        f[fy] = fx;
        w[fy] = w[x] - w[y] + diff;
        siz[fx] += siz[fy];
        return true;
    }

    int diff(int x, int y) {
        if (find(x) != find(y)) return 1e9;
        return w[y] - w[x];
    }
};
