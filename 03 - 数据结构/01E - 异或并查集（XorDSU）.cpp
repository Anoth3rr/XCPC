/*
用途：维护形如 value[x] xor value[y] = d 的二元异或约束。
约定：点编号 0..n-1，d 为 0 或 1；w[x] 表示 value[x] xor value[fa[x]]。
接口：find、merge、ask；merge 返回新约束是否与已有约束相容。
复杂度：均摊 O(alpha(n))。
*/
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
