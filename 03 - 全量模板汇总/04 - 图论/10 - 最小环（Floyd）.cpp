/*
用途：稠密非负权图的最小简单环。
MinCycle：无向图，支持自环和重边；DirCycle：有向图，允许自环和重边。
接口：add/add2 后调用 solve；返回 INF 表示无环。
复杂度：O(n^3) 时间、O(n^2) 空间。路径长度必须小于 INF，适合 n 约 500 以内。
*/
struct MinCycle {
    static constexpr int INF = numeric_limits<int>::max() / 4;

    int n, ans = INF;
    vector<vector<int>> a;

    explicit MinCycle(int n) : n(n), a(n + 1, vector<int>(n + 1, INF)) {
        for (int i = 1; i <= n; ++i) a[i][i] = 0;
    }

    void add(int u, int v, int w) {
        assert(0 <= w && w < INF);
        if (u == v) {
            ans = min(ans, w);
            return;
        }
        if (a[u][v] != INF) ans = min(ans, a[u][v] + w);
        a[u][v] = a[v][u] = min(a[u][v], w);
    }

    int solve() const {
        auto d = a;
        int res = ans;
        for (int k = 1; k <= n; ++k) {
            for (int i = 1; i < k; ++i) {
                if (a[i][k] == INF) continue;
                for (int j = 1; j < i; ++j) {
                    if (d[i][j] == INF || a[k][j] == INF) continue;
                    res = min(res, d[i][j] + a[i][k] + a[k][j]);
                }
            }
            for (int i = 1; i <= n; ++i) {
                if (d[i][k] == INF) continue;
                for (int j = 1; j <= n; ++j) {
                    if (d[k][j] == INF) continue;
                    d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
                }
            }
        }
        return res;
    }
};

struct DirCycle {
    static constexpr int INF = numeric_limits<int>::max() / 4;

    int n;
    vector<vector<int>> a;

    explicit DirCycle(int n) : n(n), a(n + 1, vector<int>(n + 1, INF)) {}

    void add(int u, int v, int w) {
        assert(0 <= w && w < INF);
        a[u][v] = min(a[u][v], w);
    }

    int solve() const {
        auto d = a;
        for (int k = 1; k <= n; ++k) {
            for (int i = 1; i <= n; ++i) {
                if (d[i][k] == INF) continue;
                for (int j = 1; j <= n; ++j) {
                    if (d[k][j] == INF) continue;
                    d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
                }
            }
        }
        int ans = INF;
        for (int i = 1; i <= n; ++i) ans = min(ans, d[i][i]);
        return ans;
    }
};
