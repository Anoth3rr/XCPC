struct ModDSU {
    vector<int> fa, siz;
    vector<i64> dis; // val[x] - val[fa[x]]
    i64 mod;

    ModDSU(int n, i64 M = 0) : fa(n + 1), siz(n + 1, 1), dis(n + 1), mod(M) {
        iota(fa.begin(), fa.end(), 0);
    }
    i64 norm(i64 x) const {
        if (!mod) return x;
        x %= mod;
        return x < 0 ? x + mod : x;
    }
    int get(int x) {
        if (x == fa[x]) return x;
        int p = fa[x];
        fa[x] = get(p);
        dis[x] = norm(dis[x] + dis[p]);
        return fa[x];
    }
    bool add(int x, int y, i64 w) { // val[x] - val[y] = w，返回新陈述是否相容
        int rx = get(x), ry = get(y);
        w = norm(w);
        if (rx == ry) return norm(dis[x] - dis[y]) == w;
        i64 rootDiff = norm(w - dis[x] + dis[y]); // val[rx] - val[ry]
        if (siz[rx] < siz[ry]) {
            fa[rx] = ry;
            dis[rx] = rootDiff;
            siz[ry] += siz[rx];
        } else {
            fa[ry] = rx;
            dis[ry] = norm(-rootDiff);
            siz[rx] += siz[ry];
        }
        return true; // 注意，即使两点已经连通，只要关系正确仍返回 true
    }
    bool same(int x, int y) {
        return get(x) == get(y);
    }
    i64 diff(int x, int y) { // 要求 same(x, y)
        get(x), get(y);
        return norm(dis[x] - dis[y]);
    }
};
