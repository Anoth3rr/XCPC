template <class T = int> struct DSU {
    vector<int> fa, sz;
    vector<T> v;
    int cc = 0;

    DSU() = default;
    explicit DSU(int n, const T &x = T()) {
        init(n, x);
    }

    void init(int n, const T &x = T()) {
        fa.resize(n + 1);
        iota(all(fa), 0);
        sz.assign(n + 1, 1);
        v.assign(n + 1, x);
        cc = n;
    }

    int find(int x) {
        return fa[x] == x ? x : fa[x] = find(fa[x]);
    }

    bool merge(int x, int y) {
        x = find(x), y = find(y);
        if (x == y) return false;
        if (sz[x] < sz[y]) swap(x, y);
        fa[y] = x;
        sz[x] += sz[y];
        v[x] += v[y];
        --cc;
        return true;
    }

    bool ask(int x, int y) {
        return find(x) == find(y);
    }

    int askSz(int x) {
        return sz[find(x)];
    }

    T &operator[](int x) {
        return v[find(x)];
    }

    int askCnt() const {
        return cc;
    }
};
