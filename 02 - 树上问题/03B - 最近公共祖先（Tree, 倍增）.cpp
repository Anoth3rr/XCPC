/*
用途：倍增版 LCA，适合静态无权树上的最近公共祖先、距离和跳祖先查询。

接口：
- work(root = 1)：预处理 dep / val / tin / tout。
- askLCA(u, v)：最近公共祖先。
- askDis(u, v)：边数距离。
- askJmp(u, k)：u 的第 k 个祖先；越过根时返回 0。
- askKth(u, v, k)：路径 u -> v 上第 k 条边到达的点；越界返回 0。

tin / tout 可直接配合“虚树”模板。点编号为 1..n。
性能：倍增祖先表默认使用 int；极限内存不足时可只将祖先编号改为 i32。
*/
struct Tree {
    int n, lg, tim;
    vector<vector<int>> ver, val;
    vector<int> dep, tin, out;

    explicit Tree(int n) : n(n), tim(0), ver(n + 1), dep(n + 1),
                           tin(n + 1), out(n + 1) {
        lg = 1;
        while ((1LL << lg) <= max<int>(1, n)) ++lg;
        val.assign(n + 1, vector<int>(lg));
    }

    void add(int u, int v) {
        ver[u].push_back(v);
        ver[v].push_back(u);
    }

    void dfs(int u, int fa) {
        val[u][0] = fa;
        for (int k = 1; k < lg; ++k) {
            val[u][k] = val[val[u][k - 1]][k - 1];
        }
        tin[u] = ++tim;
        for (int v : ver[u]) {
            if (v == fa) continue;
            dep[v] = dep[u] + 1;
            dfs(v, u);
        }
        out[u] = tim;
    }

    int askJmp(int u, int k) const {
        if (k < 0 || k > dep[u]) return 0;
        for (int bit = 0; bit < lg; ++bit) {
            if ((k >> bit) & 1) u = val[u][bit];
        }
        return u;
    }

    int askLCA(int u, int v) const {
        if (dep[u] < dep[v]) swap(u, v);
        u = askJmp(u, dep[u] - dep[v]);
        if (u == v) return u;
        for (int k = lg - 1; k >= 0; --k) {
            if (val[u][k] == val[v][k]) continue;
            u = val[u][k];
            v = val[v][k];
        }
        return val[u][0];
    }

    int askDis(int u, int v) const {
        return dep[u] + dep[v] - 2 * dep[askLCA(u, v)];
    }

    int askKth(int u, int v, int k) const {
        int p = askLCA(u, v);
        int l = dep[u] - dep[p], r = dep[v] - dep[p];
        if (k < 0 || k > l + r) return 0;
        return k <= l ? askJmp(u, k) : askJmp(v, l + r - k);
    }

    bool askAnc(int u, int v) const {
        return tin[u] <= tin[v] && out[v] <= out[u];
    }

    void work(int rt = 1) {
        tim = 0;
        fill(dep.begin(), dep.end(), 0);
        fill(tin.begin(), tin.end(), 0);
        fill(out.begin(), out.end(), 0);
        for (auto &row : val) fill(row.begin(), row.end(), 0);
        dfs(rt, 0);
    }
};
