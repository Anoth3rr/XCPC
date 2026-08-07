/*
用途：多次树上路径加法后的离线汇总，支持“加到路径点”与“加到路径边”。

接口：
- add(u, v)：加入无向边；work(root = 1) 预处理 LCA 和自顶向下 order。
- updateV(u, v, x)：给路径上所有点加 x。
- updateE(u, v, x)：给路径上所有边加 x。
- askV()：返回每个点的最终值。
- askE()：返回每个点到父亲边的最终值，root 位置恒为 0。

T 需要支持默认构造、+= 和 -=，通常直接使用 TreeDiff<int>。
所有路径更新为 O(log n)，两种 collect 均为 O(n)，且不会清空已记录的更新。
性能：倍增祖先表默认使用 int；极限内存不足时可只将祖先编号改为 i32。
*/

template <class T = int>
struct TreeDiff {
    int n, lg;
    vector<vector<int>> e, up;
    vector<int> fa, dep, ord;
    vector<T> tv, te;
    int rt0 = 1;

    explicit TreeDiff(int n)
        : n(n), e(n + 1), fa(n + 1), dep(n + 1), tv(n + 1), te(n + 1) {
        lg = 1;
        while ((1LL << lg) <= max<int>(1, n)) ++lg;
        up.assign(lg, vector<int>(n + 1));
    }

    void add(int u, int v) {
        e[u].push_back(v);
        e[v].push_back(u);
    }

    void dfs(int u, int _fa) {
        fa[u] = _fa;
        up[0][u] = _fa;
        for (int bit = 1; bit < lg; ++bit) up[bit][u] = up[bit - 1][up[bit - 1][u]];
        ord.push_back(u);
        for (int v : e[u]) {
            if (v == _fa) continue;
            dep[v] = dep[u] + 1;
            dfs(v, u);
        }
    }

    void work(int rt = 1) {
        rt0 = rt;
        fill(fa.begin(), fa.end(), 0);
        fill(dep.begin(), dep.end(), 0);
        for (auto &row : up) fill(row.begin(), row.end(), 0);
        ord.clear();
        dfs(rt0, 0);
    }

    int askJmp(int u, int k) const {
        if (k < 0 || k > dep[u]) return 0;
        for (int bit = 0; bit < lg; ++bit) {
            if ((k >> bit) & 1) u = up[bit][u];
        }
        return u;
    }

    int askLCA(int u, int v) const {
        if (dep[u] < dep[v]) swap(u, v);
        u = askJmp(u, dep[u] - dep[v]);
        if (u == v) return u;
        for (int bit = lg - 1; bit >= 0; --bit) {
            if (up[bit][u] == up[bit][v]) continue;
            u = up[bit][u];
            v = up[bit][v];
        }
        return fa[u];
    }

    void clear() {
        fill(tv.begin(), tv.end(), T{});
        fill(te.begin(), te.end(), T{});
    }

    void updateV(int u, int v, const T &d) {
        int p = askLCA(u, v);
        tv[u] += d;
        tv[v] += d;
        tv[p] -= d;
        if (fa[p]) tv[fa[p]] -= d;
    }

    void updateE(int u, int v, const T &d) {
        int p = askLCA(u, v);
        te[u] += d;
        te[v] += d;
        te[p] -= d;
        te[p] -= d;
    }

    vector<T> collect(vector<T> val) const {
        for (int i = ord.size(); i--;) {
            int u = ord[i];
            if (fa[u]) val[fa[u]] += val[u];
        }
        return val;
    }

    vector<T> askV() const {
        return collect(tv);
    }

    vector<T> askE() const {
        vector<T> res = collect(te);
        res[rt0] = T{};
        return res;
    }
};
