/*
用途：带势能（权值）并查集，维护形如 potential[x] - potential[y] = diff 的约束。

w[x] 表示 potential[x] - potential[fa[x]]；ask(x) 返回 x 相对所在根的势能。
merge(x,y,d)：加入 potential[x] - potential[y] = d，
若两点已连通则返回该约束是否与已有约束一致。

T 通常为 int 或 MInt，需支持 +、-、== 和默认构造。
*/
template <class T>
struct DSU {
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
