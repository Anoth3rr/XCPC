/*
用途：倍增版有权树 LCA，支持路径距离、路径最大边权和跳祖先查询。

接口：
- add(u, v, w)：加入权值为 w 的无向边（w 可为负）。
- work(root = 1)：预处理 dep / dis / val / mx / tin / out。
- askLCA(u, v)、askDis(u, v)、askMax(u, v)、askJmp(u, k)、askKth(u, v, k)。

askDis 返回路径权值和；askMax 返回路径最大边权。对空路径（u == v），askMax 返回 0。
性能：倍增祖先表默认使用 int；极限内存不足时可只将祖先编号改为 i32，边权与距离保持 int。
*/

struct Tree {
    static constexpr int NI = numeric_limits<int>::lowest() / 4;

    int n, lg, tim;
    vector<vector<int>> val;
    vector<vector<int>> mx;
    vector<vector<pair<int, int>>> ver;
    vector<int> dep, tin, out;
    vector<int> dis;

    explicit Tree(int n) : n(n), tim(0), ver(n + 1), dep(n + 1), tin(n + 1),
                           out(n + 1), dis(n + 1) {
        lg = 1;
        while ((1LL << lg) <= max<int>(1, n)) ++lg;
        val.assign(n + 1, vector<int>(lg));
        mx.assign(n + 1, vector<int>(lg, NI));
    }

    void add(int u, int v, int w) {
        ver[u].push_back({v, w});
        ver[v].push_back({u, w});
    }

    void dfs(int u, int fa) {
        val[u][0] = fa;
        for (int k = 1; k < lg; ++k) {
            val[u][k] = val[val[u][k - 1]][k - 1];
            mx[u][k] = max(mx[u][k - 1], mx[val[u][k - 1]][k - 1]);
        }
        tin[u] = ++tim;
        for (auto [v, w] : ver[u]) {
            if (v == fa) continue;
            dep[v] = dep[u] + 1;
            dis[v] = dis[u] + w;
            mx[v][0] = w;
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
        int p = askLCA(u, v);
        return dis[u] + dis[v] - 2 * dis[p];
    }

    int askMax(int u, int v) const {
        if (u == v) return 0;
        int ans = NI;
        auto up = [&](int x, int t) {
            for (int k = lg - 1; k >= 0; --k) {
                if (dep[x] - (1LL << k) >= dep[t]) {
                    ans = max(ans, mx[x][k]);
                    x = val[x][k];
                }
            }
        };

        int p = askLCA(u, v);
        up(u, p);
        up(v, p);
        return ans;
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
        fill(dis.begin(), dis.end(), 0);
        for (auto &row : val) fill(row.begin(), row.end(), 0);
        for (auto &row : mx) fill(row.begin(), row.end(), NI);
        dfs(rt, 0);
    }
};
