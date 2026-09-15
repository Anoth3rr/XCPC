/*
用途：邻接矩阵 Dijkstra，适合稠密非负权图。
用法：Dijkstra<> g(n)；add/add2 后 solve(s, &fa)。
约定：点号为 1..n；fa 非空时记录一棵最短路树。
复杂度：O(n^2) 时间、O(n^2) 空间。稀疏图优先 01A 的堆优化版。
性能：默认 int 为 long long 会使邻接矩阵翻倍；仅在实际内存受限且权值可装 i32 时再局部缩窄。
*/

template <class T = int> struct Dijkstra {
    static constexpr T INF = numeric_limits<T>::max() / 4;

    int n;
    vector<vector<T>> g;

    explicit Dijkstra(int n) : n(n), g(n + 1, vector<T>(n + 1, INF)) {}

    void add(int u, int v, T w) {
        assert(w >= 0);
        if (w < g[u][v]) g[u][v] = w;
    }

    void add2(int u, int v, T w) {
        add(u, v, w);
        add(v, u, w);
    }

    vector<T> solve(int s, vector<int>* fa = nullptr) const {
        vector<T> dis(n + 1, INF);
        vector<char> vis(n + 1);
        if (fa) fa->assign(n + 1, -1);
        dis[s] = 0;
        for (int i = 1; i <= n; ++i) {
            int u = 0;
            for (int v = 1; v <= n; ++v) {
                if (!vis[v] && (u == 0 || dis[v] < dis[u])) u = v;
            }
            if (u == 0 || dis[u] == INF) break;
            vis[u] = true;
            for (int v = 1; v <= n; ++v) {
                if (vis[v] || g[u][v] == INF || dis[v] <= dis[u] + g[u][v]) continue;
                dis[v] = dis[u] + g[u][v];
                if (fa) (*fa)[v] = u;
            }
        }
        return dis;
    }
};
