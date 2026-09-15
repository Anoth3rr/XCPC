/*
用途：有向无环图（DAG）的拓扑排序与判环。
约定：点编号 1..n，add 添加有向边。
复杂度：O(n + m)。solve() 返回 nullopt 表示图中存在有向环。
*/
struct Topo {
    int n;
    vector<vector<int>> g;
    vector<int> in;

    explicit Topo(int n) : n(n), g(n + 1), in(n + 1) {}

    void add(int u, int v) {
        g[u].push_back(v);
        ++in[v];
    }

    optional<vector<int>> solve() const {
        vector<int> deg = in;
        queue<int> q;
        for (int u = 1; u <= n; ++u) {
            if (deg[u] == 0) q.push(u);
        }

        vector<int> ord;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            ord.push_back(u);
            for (int v : g[u]) {
                if (--deg[v] == 0) q.push(v);
            }
        }
        if (ord.size() != n) return nullopt;
        return ord;
    }
};
