/*
用途：非负边权单源最短路。
用法：Dijkstra<> g(n)；若权值类型不同写 Dijkstra<T>。
约定：点号和父亲数组使用 int，距离类型由模板 T 决定。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/

template <class T = int> struct Dijkstra {
    static constexpr T INF = numeric_limits<T>::max() / 4;

    struct Edge {
        int v;
        T w;
    };

    int n;
    vector<vector<Edge>> g;

    explicit Dijkstra(int n) : n(n), g(n + 1) {
        assert(n >= 0);
    }

    void add(int u, int v, T w) {
        assert(w >= 0);
        g[u].push_back({v, w});
    }

    void add2(int u, int v, T w) {
        add(u, v, w);
        add(v, u, w);
    }

    vector<T> solve(int s, vector<int>* fa = nullptr) const {
        vector<T> dis(n + 1, INF);
        if (fa) fa->assign(n + 1, -1);
        using S = pair<T, int>;
        priority_queue<S, vector<S>, greater<S>> q;
        dis[s] = 0;
        q.push({0, s});
        while (!q.empty()) {
            auto [d, u] = q.top();
            q.pop();
            if (d != dis[u]) continue;
            for (auto [v, w] : g[u]) {
                if (dis[v] <= d + w) continue;
                dis[v] = d + w;
                if (fa) (*fa)[v] = u;
                q.push({dis[v], v});
            }
        }
        return dis;
    }
};
