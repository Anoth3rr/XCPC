/*
用途：稀疏图全源最短路，允许负边；有负环时返回 nullopt。
复杂度：O(nm+n(m+n)log n)，空间 O(n^2+m)。点号 1..n。
约定：T 为有符号距离类型；边权和任意简单路长度应落在 T 的安全范围内。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/

template <class T = int> struct Johnson {
    static constexpr T INF = numeric_limits<T>::max() / 4;
    struct E { int u, v; T w; };
    int n;
    vector<E> e;
    vector<vector<pair<int, T>>> g;

    explicit Johnson(int _n) : n(_n), g(_n + 1) {
        static_assert(is_signed_v<T>);
        assert(n >= 0);
    }
    void add(int u, int v, T w) {
        assert(1 <= u && u <= n && 1 <= v && v <= n);
        e.push_back({u, v, w});
    }

    optional<vector<vector<T>>> solve() {
        vector<T> h(n + 1);
        for (int k = 1; k <= n; ++k) {
            bool f = false;
            for (auto [u, v, w] : e) if (h[v] > h[u] + w)
                h[v] = h[u] + w, f = true;
            if (!f) break;
            if (k == n) return nullopt;
        }
        g.assign(n + 1, {});
        for (auto [u, v, w] : e) {
            T x = w + h[u] - h[v];
            assert(x >= 0);
            g[u].push_back({v, x});
        }

        vector<vector<T>> a(n + 1, vector<T>(n + 1, INF));
        for (int s = 1; s <= n; ++s) {
            vector<T> d(n + 1, INF);
            priority_queue<pair<T, int>, vector<pair<T, int>>, greater<pair<T, int>>> q;
            d[s] = 0, q.push({0, s});
            while (!q.empty()) {
                auto [du, u] = q.top(); q.pop();
                if (du != d[u]) continue;
                for (auto [v, w] : g[u]) if (du <= INF - w && d[v] > du + w)
                    d[v] = du + w, q.push({d[v], v});
            }
            for (int v = 1; v <= n; ++v) if (d[v] != INF) a[s][v] = d[v] - h[s] + h[v];
        }
        return a;
    }
};
