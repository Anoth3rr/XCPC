/*
用途：允许负边权的单源最短路；若存在源点可达负环则返回 nullopt。
约定：点编号 1..n，边均为有向边；最坏复杂度 O(nm)，通常仅在数据温和时使用。
所有有限距离应能放进 int。
*/

struct SPFA {
    static constexpr int INF = numeric_limits<int>::max() / 4;

    struct Edge {
        int v;
        int w;
    };

    int n;
    vector<vector<Edge>> g;

    explicit SPFA(int n) : n(n), g(n + 1) {}

    void add(int u, int v, int w) {
        g[u].push_back({v, w});
    }

    // 返回 nullopt 表示存在从 source 可达的负环。
    optional<vector<int>> solve(int s) const {
        vector<int> dis(n + 1, INF);
        vector<int> m(n + 1);
        vector<char> inq(n + 1, false);
        queue<int> q;

        dis[s] = 0;
        q.push(s);
        inq[s] = true;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            inq[u] = false;

            for (auto [v, w] : g[u]) {
                if (dis[v] <= dis[u] + w) continue;
                dis[v] = dis[u] + w;
                m[v] = m[u] + 1;
                if (m[v] >= n) return nullopt;
                if (!inq[v]) {
                    inq[v] = true;
                    q.push(v);
                }
            }
        }
        return dis;
    }
};
