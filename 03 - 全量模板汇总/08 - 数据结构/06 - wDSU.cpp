template <class T> struct wDSU {
    vector<int> f, siz;
    vector<T> dis;

    wDSU(int n) : f(n + 1), siz(n + 1, 1), dis(n + 1) {
        iota(f.begin(), f.end(), 0);
    }
    int find(int x) {
        if (x == f[x]) return x;
        int p = f[x];
        f[x] = find(p);
        dis[x] = dis[x] + dis[p];
        return f[x];
    }
    bool add(int x, int y, T w) {
        int rx = find(x), ry = find(y);
        if (rx == ry) return dis[x] - dis[y] == w;
        T d = w - dis[x] + dis[y];
        if (siz[rx] < siz[ry]) {
            f[rx] = ry;
            dis[rx] = d;
            siz[ry] += siz[rx];
        } else {
            f[ry] = rx;
            dis[ry] = T{} - d;
            siz[rx] += siz[ry];
        }
        return true;
    }
    bool same(int x, int y) {
        return find(x) == find(y);
    }
    T diff(int x, int y) {
        find(x), find(y);
        return dis[x] - dis[y];
    }
};