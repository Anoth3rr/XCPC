/*
用途：无向带权图的最小生成森林；ok 为真时即最小生成树。
约定：点编号 1..n，可含重边和负边权。
复杂度：O(m log n)。稠密图通常可改为 O(n^2) 朴素 Prim。
*/

struct MST {

    struct Edge {
        int v;
        int w;
    };

    struct CEdge {
        int u, v;
        int w;
    };

    struct Result {
        bool ok;
        int w;                    // 非连通时为最小生成森林的权值。
        vector<CEdge> vis;  // 非连通时为最小生成森林的边。
    };

    int n;
    vector<vector<Edge>> g;

    explicit MST(int n) : n(n), g(n + 1) {}

    void add(int u, int v, int w) {
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }

    Result solve() const {
        using State = tuple<int, int, int>;  // weight, vertex, parent
        priority_queue<State, vector<State>, greater<State>> q;
        vector<char> use(n + 1, false);
        vector<CEdge> vis;
        int sum = 0;
        int cc = 0;

        for (int s = 1; s <= n; ++s) {
            if (use[s]) continue;
            ++cc;
            q.push({0, s, 0});
            while (!q.empty()) {
                auto [w, u, fa] = q.top();
                q.pop();
                if (use[u]) continue;
                use[u] = true;
                sum += w;
                if (fa != 0) vis.push_back({fa, u, w});

                for (auto [v, w] : g[u]) {
                    if (!use[v]) q.push({w, v, u});
                }
            }
        }
        return {cc <= 1, sum, move(vis)};
    }
};
