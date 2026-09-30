struct rDSU {
    vector<int> f, siz;
    vector<pii> his;
    int cnt;

    rDSU(int n) : f(n + 1), siz(n + 1, 1), cnt(n) {
        iota(f.begin(), f.end(), 0);
    }
    int find(int x) {
        while (x != f[x]) x = f[x];
        return x;
    }
    bool merge(int x, int y) {
        x = find(x), y = find(y);
        if (x == y) return false;
        if (siz[x] < siz[y]) swap(x, y);
        his.push_back({x, y});
        f[y] = x;
        siz[x] += siz[y];
        cnt--;
        return true;
    }
    int time() const { return his.size(); }
    void revert(int t) {
        while (time() > t) {
            auto [x, y] = his.back();
            his.pop_back();
            f[y] = y;
            siz[x] -= siz[y];
            cnt++;
        }
    }
    bool same(int x, int y) { return find(x) == find(y); }
    int size(int x) { return siz[find(x)]; }
};