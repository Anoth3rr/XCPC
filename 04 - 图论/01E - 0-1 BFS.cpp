/*
用途：边权只为 0 或 1 时的单源最短路，比 Dijkstra 更轻。
约定：点编号 1..n；add 加有向边，add2 加无向边。
复杂度：O(n + m)。
*/

struct ZeroOneBFS {
    static constexpr int INF = numeric_limits<int>::max() / 4;

    struct Edge {
        int v;
        int w;
    };

    int n;
    vector<vector<Edge>> g;

    explicit ZeroOneBFS(int n) : n(n), g(n + 1) {}

    void add(int u, int v, int w) {
        assert(w == 0 || w == 1);
        g[u].push_back({v, w});
    }

    void add2(int u, int v, int w) {
        add(u, v, w);
        add(v, u, w);
    }

    vector<int> solve(int s) const {
        vector<int> dis(n + 1, INF);
        deque<int> q;
        dis[s] = 0;
        q.push_back(s);

        while (!q.empty()) {
            int u = q.front();
            q.pop_front();
            for (auto [v, w] : g[u]) {
                if (dis[v] <= dis[u] + w) continue;
                dis[v] = dis[u] + w;
                if (w == 0) {
                    q.push_front(v);
                } else {
                    q.push_back(v);
                }
            }
        }
        return dis;
    }
};
