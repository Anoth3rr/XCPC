/*
用途：可撤销并查集。适合线段树分治、离线动态连通性等场景。

注意：find 不做路径压缩，否则无法回滚；按大小合并保证单次 find 为 O(log n)。
接口：time、merge、undo、ask、askSz、askCnt；undo(t) 回到时刻 t。
*/
struct DSU {
    struct Ch {
        int x, y, sz;
    };

    vector<int> fa, sz;
    vector<Ch> his;
    int cc = 0;

    DSU() = default;
    explicit DSU(int n) {
        init(n);
    }

    void init(int n) {
        fa.resize(n + 1);
        iota(all(fa), 0);
        sz.assign(n + 1, 1);
        his.clear();
        cc = n;
    }

    int find(int x) const {
        while (fa[x] != x) x = fa[x];
        return x;
    }

    bool merge(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) return false;
        if (sz[x] < sz[y]) swap(x, y);
        his.push_back({x, y, sz[x]});
        fa[y] = x;
        sz[x] += sz[y];
        --cc;
        return true;
    }

    int time() const {
        return his.size();
    }

    void undo(int t) {
        assert(0 <= t && t <= his.size());
        while (his.size() > t) {
            auto [x, y, s] = his.back();
            his.pop_back();
            fa[y] = y;
            sz[x] = s;
            ++cc;
        }
    }

    bool ask(int x, int y) const {
        return find(x) == find(y);
    }

    int askSz(int x) const {
        return sz[find(x)];
    }

    int askCnt() const {
        return cc;
    }
};
