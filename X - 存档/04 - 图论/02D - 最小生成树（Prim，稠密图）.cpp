/*
用途：邻接矩阵 Prim，求无向带权图的最小生成森林。
用法：MST g(n)；add(u,v,w) 后 solve()，ok 为真时原图连通。
复杂度：O(n^2) 时间、O(n^2) 空间。稀疏图优先 02A 或 02B。
性能：邻接矩阵受 #define int long long 影响显著；只在实际内存紧张且权值可装 i32 时缩窄。
*/

struct MST {
    static constexpr int INF = numeric_limits<int>::max() / 4;

    struct CEdge {
        int u, v, w;
    };

    struct Result {
        bool ok;
        int w;
        vector<CEdge> vis;
    };

    int n;
    vector<vector<int>> g;

    explicit MST(int n) : n(n), g(n + 1, vector<int>(n + 1, INF)) {}

    void add(int u, int v, int w) {
        if (w < g[u][v]) {
            g[u][v] = w;
            g[v][u] = w;
        }
    }

    Result solve() const {
        vector<int> dis(n + 1, INF), fa(n + 1);
        vector<char> use(n + 1);
        vector<CEdge> vis;
        int sum = 0, cc = 0;

        for (int i = 1; i <= n; ++i) {
            int u = 0;
            for (int v = 1; v <= n; ++v) {
                if (!use[v] && (u == 0 || dis[v] < dis[u])) u = v;
            }
            if (dis[u] == INF) {
                dis[u] = 0;
                fa[u] = 0;
                ++cc;
            }
            use[u] = true;
            sum += dis[u];
            if (fa[u]) vis.push_back({fa[u], u, dis[u]});

            for (int v = 1; v <= n; ++v) {
                if (!use[v] && g[u][v] < dis[v]) {
                    dis[v] = g[u][v];
                    fa[v] = u;
                }
            }
        }
        return {cc <= 1, sum, move(vis)};
    }
};
