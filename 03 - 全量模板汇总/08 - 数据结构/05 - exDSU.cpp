struct exDSU {
    vector<int> f, siz, e;
    int cnt;

    exDSU(int n) : f(n + 1), siz(n + 1, 1), e(n + 1), cnt(n) {
        iota(f.begin(), f.end(), 0);
    }
    int find(int x) {
        while (x != f[x]) x = f[x] = f[f[x]];
        return x;
    }
    bool add(int x, int y) {
        x = find(x), y = find(y);
        if (x == y) {
            e[x]++;
            return false;
        }
        if (siz[x] < siz[y]) swap(x, y);
        f[y] = x;
        siz[x] += siz[y];
        e[x] += e[y] + 1;
        cnt--;
        return true;
    }
    bool same(int x, int y) { return find(x) == find(y); }
    int size(int x) { return siz[find(x)]; }
    int getEdge(int x) { return e[find(x)]; }
    bool isTree(int x) {
        x = find(x);
        return e[x] == siz[x] - 1;
    }
};