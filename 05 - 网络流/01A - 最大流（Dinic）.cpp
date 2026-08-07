/*
用途：Dinic 最大流、残量图最小割恢复；作为短代码与通用默认分支。
约定：容量类型用 Cap 模板适配；点号和反向边下标使用 int。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/

template <class Cap = int> struct MaxFlow {
    static constexpr Cap INF = numeric_limits<Cap>::max() / 4;

    struct Edge {
        int v, r;
        Cap c, oc;
    };

    struct EdgeInfo {
        int u, v;
        Cap c, f;
    };

    int n;
    vector<vector<Edge>> g;
    vector<array<int, 2>> pos;
    vector<int> dep, cur;

    explicit MaxFlow(int n) : n(n), g(n) {
        assert(n >= 0);
    }

    int add(int u, int v, Cap c) {
        assert(0 <= u && u < n && 0 <= v && v < n && c >= 0);
        int x = g[u].size();
        int y = g[v].size() + (u == v);
        g[u].push_back({v, y, c, c});
        g[v].push_back({u, x, 0, 0});
        pos.push_back({u, x});
        return pos.size() - 1;
    }

    void add2(int u, int v, Cap c) {
        add(u, v, c);
        add(v, u, c);
    }

    void reset() {
        for (auto& a : g)
            for (Edge& e : a) e.c = e.oc;
    }

    EdgeInfo askEdge(int id) const {
        assert(0 <= id && id < pos.size());
        auto [u, i] = pos[id];
        const Edge& e = g[u][i];
        return {u, e.v, e.oc, e.oc - e.c};
    }

    Cap askFlow(int id) const {
        return askEdge(id).f;
    }

    vector<EdgeInfo> askAll() const {
        vector<EdgeInfo> a;
        a.reserve(pos.size());
        for (int i = 0; i < pos.size(); ++i) a.push_back(askEdge(i));
        return a;
    }

    void erase(int id) {
        assert(0 <= id && id < pos.size());
        auto [u, i] = pos[id];
        Edge& e = g[u][i];
        Edge& f = g[e.v][e.r];
        e.c = f.c = 0;
    }

    bool bfs(int s, int t) {
        dep.assign(n, -1);
        queue<int> q;
        dep[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (const Edge& e : g[u]) {
                if (e.c <= 0 || dep[e.v] != -1) continue;
                dep[e.v] = dep[u] + 1;
                q.push(e.v);
            }
        }
        return dep[t] != -1;
    }

    Cap dfs(int u, int t, Cap f) {
        if (u == t || !f) return f;
        for (int& i = cur[u]; i < g[u].size(); ++i) {
            Edge& e = g[u][i];
            if (e.c <= 0 || dep[e.v] != dep[u] + 1) continue;
            Cap x = dfs(e.v, t, min(f, e.c));
            if (!x) continue;
            e.c -= x;
            g[e.v][e.r].c += x;
            return x;
        }
        return 0;
    }

    Cap flow(int s, int t, Cap lim = INF) {
        assert(0 <= s && s < n && 0 <= t && t < n && s != t && lim >= 0);
        Cap r = 0;
        while (r < lim && bfs(s, t)) {
            cur.assign(n, 0);
            while (r < lim) {
                Cap x = dfs(s, t, min(lim - r, INF));
                if (!x) break;
                r += x;
            }
        }
        return r;
    }

    vector<char> askCut(int s) const {
        assert(0 <= s && s < n);
        vector<char> a(n);
        queue<int> q;
        q.push(s);
        a[s] = 1;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (const Edge& e : g[u]) {
                if (e.c > 0 && !a[e.v]) {
                    a[e.v] = 1;
                    q.push(e.v);
                }
            }
        }
        return a;
    }
};
