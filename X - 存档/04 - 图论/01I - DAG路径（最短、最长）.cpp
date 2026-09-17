/*
用途：DAG 上从单源出发的最短/最长路，可含负边。
用法：DAG<int> g(n)；add(u,v,w) 后调用 askMin(s) 或 askMax(s)。
接口：若存在环，askMin/askMax 返回 nullopt；否则返回所有点的距离。
复杂度：O(n+m)。不可达点分别为 INF / LOW。
*/

template <class T = int> struct DAG {
    static constexpr T INF = numeric_limits<T>::max() / 4;
    static constexpr T LOW = numeric_limits<T>::lowest() / 4;

    struct Edge {
        int v;
        T w;
    };

    int n;
    vector<vector<Edge>> g;
    vector<int> deg;

    explicit DAG(int n) : n(n), g(n + 1), deg(n + 1) {}

    void add(int u, int v, T w) {
        g[u].push_back({v, w});
        ++deg[v];
    }

    vector<int> topo() const {
        vector<int> d = deg, ord;
        queue<int> q;
        for (int u = 1; u <= n; ++u) {
            if (!d[u]) q.push(u);
        }
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            ord.push_back(u);
            for (auto [v, w] : g[u]) {
                if (!--d[v]) q.push(v);
            }
        }
        return ord;
    }

    optional<vector<T>> askMin(int s, vector<int>* fa = nullptr) const {
        vector<int> ord = topo();
        if (ord.size() != deg.size() - 1) return nullopt;
        vector<T> dis(n + 1, INF);
        if (fa) fa->assign(n + 1, -1);
        dis[s] = 0;
        for (int u : ord) {
            if (dis[u] == INF) continue;
            for (auto [v, w] : g[u]) {
                if (dis[v] <= dis[u] + w) continue;
                dis[v] = dis[u] + w;
                if (fa) (*fa)[v] = u;
            }
        }
        return dis;
    }

    optional<vector<T>> askMax(int s, vector<int>* fa = nullptr) const {
        vector<int> ord = topo();
        if (ord.size() != deg.size() - 1) return nullopt;
        vector<T> dis(n + 1, LOW);
        if (fa) fa->assign(n + 1, -1);
        dis[s] = 0;
        for (int u : ord) {
            if (dis[u] == LOW) continue;
            for (auto [v, w] : g[u]) {
                if (dis[v] >= dis[u] + w) continue;
                dis[v] = dis[u] + w;
                if (fa) (*fa)[v] = u;
            }
        }
        return dis;
    }
};
