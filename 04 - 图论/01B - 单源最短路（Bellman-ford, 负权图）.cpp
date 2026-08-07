/*
用途：允许负边权的单源最短路，并标记从源点可达的负环所能影响的点。
约定：点编号 1..n，边均为有向边；所有有限距离应能放进 int。
复杂度：O(nm)。负环不影响的点，其 distance 才有最短路意义。
*/

struct Bellman {
    static constexpr int INF = numeric_limits<int>::max() / 4;

    struct Edge {
        int u, v;
        int w;
    };

    struct Result {
        vector<int> dis;
        vector<char> neg;

        bool askNeg() const {
            return any_of(neg.begin() + 1,
                          neg.end(), [](char v) { return v; });
        }
    };

    int n;
    vector<Edge> e;
    vector<vector<int>> g;

    explicit Bellman(int n) : n(n), g(n + 1) {}

    void add(int u, int v, int w) {
        e.push_back({u, v, w});
        g[u].push_back(v);
    }

    Result solve(int s) const {
        vector<int> dis(n + 1, INF);
        dis[s] = 0;

        for (int k = 1; k < n; ++k) {
            bool chg = false;
            for (auto [u, v, w] : e) {
                if (dis[u] == INF) continue;
                if (dis[v] > dis[u] + w) {
                    dis[v] = dis[u] + w;
                    chg = true;
                }
            }
            if (!chg) break;
        }

        vector<char> neg(n + 1, false);
        queue<int> q;
        for (auto [u, v, w] : e) {
            if (dis[u] != INF && dis[v] > dis[u] + w && !neg[v]) {
                neg[v] = true;
                q.push(v);
            }
        }
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : g[u]) {
                if (!neg[v]) {
                    neg[v] = true;
                    q.push(v);
                }
            }
        }
        return {move(dis), move(neg)};
    }
};
