/*
用途：Kruskal 重构树（升序），把“边权不超过 w 时的连通性”转化为树上的 LCA。

build 后：
- askMin(u, v) 返回 u、v 连通所需的最小阈值，即原图 u-v 路径中“最大边权最小”的值；
- askLCA(u, v) 对应两点首次被 Kruskal 合并时的重构树节点；
- sz[askLCA(u, v)] 是该时刻所在连通块大小；
- askDisc(u, v) 判断原图是否连通。

输入可为非连通图，点编号为 1..n。边按权值升序处理；相同权值的合并顺序不影响 askMin，
但若题目需要“同权边同时合并”的连通块语义，需要将同权边额外缩点后再建树。
性能：倍增祖先表默认使用 int；极限内存不足时可只将祖先编号改为 i32，权值保持 int。
*/

struct KRT {

    struct Edge {
        int u, v;
        int w;
    };

    int n, tot = 0, lg = 1;
    vector<int> dsu, bel, sz, fa, dep;
    vector<vector<int>> son, up;
    vector<int> val;
    vector<int> rts;

    explicit KRT(int n) : n(n) {
        reset();
    }

    void reset() {
        int cap = 2 * n + 5;
        tot = n;
        dsu.resize(cap);
        bel.assign(n + 1, 0);
        sz.assign(cap, 0);
        fa.assign(cap, 0);
        dep.assign(cap, 0);
        son.assign(cap, {});
        val.assign(cap, 0);
        rts.clear();
        for (int u = 1; u <= n; ++u) dsu[u] = u, sz[u] = 1;
    }

    int find(int x) {
        return dsu[x] == x ? x : dsu[x] = find(dsu[x]);
    }

    void dfs(int u, int _fa) {
        fa[u] = _fa;
        up[0][u] = _fa;
        for (int bit = 1; bit < lg; ++bit) up[bit][u] = up[bit - 1][up[bit - 1][u]];
        for (int v : son[u]) {
            dep[v] = dep[u] + 1;
            dfs(v, u);
        }
    }

    void build(vector<Edge> es) {
        reset();
        sort(es.begin(), es.end(), [](const Edge &a, const Edge &b) { return a.w < b.w; });
        for (const auto &[u, v, w] : es) {
            int x = find(u), y = find(v);
            if (x == y) continue;
            int now = ++tot;
            val[now] = w;
            son[now] = {x, y};
            sz[now] = sz[x] + sz[y];
            dsu[x] = dsu[y] = now;
            dsu[now] = now;
        }

        for (int u = 1; u <= n; ++u) bel[u] = find(u);
        rts.assign(bel.begin() + 1, bel.end());
        sort(rts.begin(), rts.end());
        rts.erase(unique(rts.begin(), rts.end()), rts.end());

        lg = 1;
        while ((1LL << lg) <= max<int>(1, tot)) ++lg;
        up.assign(lg, vector<int>(tot + 1));
        fill(fa.begin(), fa.end(), 0);
        fill(dep.begin(), dep.end(), 0);
        for (int r : rts) dfs(r, 0);
    }

    bool askDisc(int u, int v) const {
        return bel[u] != bel[v];
    }

    int askLCA(int u, int v) const {
        if (askDisc(u, v)) return 0;
        if (dep[u] < dep[v]) swap(u, v);
        int dif = dep[u] - dep[v];
        for (int bit = 0; bit < lg; ++bit) {
            if ((dif >> bit) & 1) u = up[bit][u];
        }
        if (u == v) return u;
        for (int bit = lg - 1; bit >= 0; --bit) {
            if (up[bit][u] == up[bit][v]) continue;
            u = up[bit][u];
            v = up[bit][v];
        }
        return up[0][u];
    }

    optional<int> askMin(int u, int v) const {
        if (askDisc(u, v)) return nullopt;
        if (u == v) return 0;
        return val[askLCA(u, v)];
    }
};
