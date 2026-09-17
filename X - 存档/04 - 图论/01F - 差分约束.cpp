/*
用途：差分约束系统。addLE(u,v,w) 表示 x[v]-x[u]<=w，对应边 u->v 权 w。
solve 返回一组可行解；存在矛盾（负环）时返回 nullopt。变量编号 0..n-1。
复杂度：SPFA 平均较快，最坏 O(nm)；对抗数据可改用 Bellman-Ford。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/

template <class T = int> struct DiffCons {
    struct Edge {
        int v;
        T w;
    };

    int n;
    vector<vector<Edge>> g;
    explicit DiffCons(int n) : n(n), g(n) { assert(n >= 0); }

    void addLE(int u, int v, T w) {
        assert(0 <= u && u < n && 0 <= v && v < n);
        g[u].push_back({v, w});
    }
    void addGE(int u, int v, T w) { addLE(v, u, -w); }
    void addEQ(int u, int v, T w) {
        addLE(u, v, w);
        addGE(u, v, w);
    }

    optional<vector<T>> solve() const {
        vector<T> d(n, T{});
        vector<int> len(n), q;
        vector<char> in(n, true);
        q.reserve(n * 2);
        for (int i = 0; i < n; ++i) q.push_back(i);  // 等价于权 0 超级源。
        for (int h = 0; h < q.size(); ++h) {
            int u = q[h];
            in[u] = false;
            for (auto [v, w] : g[u]) {
                if (d[v] <= d[u] + w) continue;
                d[v] = d[u] + w;
                len[v] = len[u] + 1;
                if (len[v] >= n) return nullopt;
                if (!in[v]) in[v] = true, q.push_back(v);
            }
            if (h > 2 * n && h * 2 > q.size()) {
                q.erase(q.begin(), q.begin() + h + 1);
                h = -1;
            }
        }
        return d;
    }
};
