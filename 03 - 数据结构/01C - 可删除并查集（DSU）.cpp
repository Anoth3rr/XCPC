/*
用途：可删除并查集（“替身节点”技巧）。

erase(x) 把原编号 x 从当前集合移出，并让 x 重新成为一个大小为 1 的单点集合；
此前的内部节点仍保留在旧集合中，但不再代表任何当前编号。适合“合并集合 + 删除元素”。

接口：merge(x,y)、erase(x)、ask(x,y)、askSz(x)。点编号为 1..n。
*/
struct DSU {
    vector<int> fa, sz, id;
    int tot = 0;

    DSU() = default;
    explicit DSU(int n) {
        init(n);
    }

    void init(int n) {
        tot = n;
        fa.resize(n + 1);
        iota(all(fa), 0);
        sz.assign(n + 1, 1);
        id.resize(n + 1);
        for (int i = 1; i <= n; ++i) id[i] = i;
    }

    int find(int x) {
        return fa[x] == x ? x : fa[x] = find(fa[x]);
    }

    int node() {
        ++tot;
        fa.push_back(tot);
        sz.push_back(1);
        return tot;
    }

    bool merge(int x, int y) {
        int a = find(id[x]);
        int b = find(id[y]);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        fa[b] = a;
        sz[a] += sz[b];
        return true;
    }

    bool link(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        fa[b] = a;
        sz[a] += sz[b];
        return true;
    }

    void erase(int x) {
        --sz[find(id[x])];
        id[x] = node();
    }

    bool ask(int x, int y) {
        return find(id[x]) == find(id[y]);
    }

    int askSz(int x) {
        return sz[find(id[x])];
    }
};
