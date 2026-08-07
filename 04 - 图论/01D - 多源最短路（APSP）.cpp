/*
用途：Floyd-Warshall 全源最短路。
约定：点编号 1..n；允许负边权，但不允许负环。调用 solve 后 dis[i][j] 即答案。
复杂度：O(n^3) 时间、O(n^2) 空间，适合 n 约 500 以内的稠密图。
*/

struct Floyd {
    static constexpr int INF = numeric_limits<int>::max() / 4;

    int n;
    vector<vector<int>> dis;

    explicit Floyd(int n) : n(n), dis(n + 1, vector<int>(n + 1, INF)) {
        for (int i = 1; i <= n; ++i) dis[i][i] = 0;
    }

    void add(int u, int v, int w) {
        dis[u][v] = min(dis[u][v], w);
    }

    void add2(int u, int v, int w) {
        add(u, v, w);
        add(v, u, w);
    }

    void solve() {
        for (int k = 1; k <= n; ++k) {
            for (int i = 1; i <= n; ++i) {
                if (dis[i][k] == INF) continue;
                for (int j = 1; j <= n; ++j) {
                    if (dis[k][j] == INF) continue;
                    dis[i][j] = min(dis[i][j], dis[i][k] + dis[k][j]);
                }
            }
        }
    }

    bool askNeg() const {
        for (int i = 1; i <= n; ++i) {
            if (dis[i][i] < 0) return true;
        }
        return false;
    }
};
