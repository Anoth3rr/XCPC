/*
用途：无向非负权图中连接给定终端点的最小 Steiner 树（允许经过非终端点）。
约定：点编号 1..n；终端点应互异；k<=20；不连通时返回 nullopt。
复杂度：O(3^k n + 2^k m log n)，空间 O(2^k n)。
注意：这是“终端点较少”的精确算法；k 大时应改用题目专用结构或近似算法。
*/
template <class T = int> struct Steiner {
    struct E {
        int v;
        T w;
    };

    int n;
    vector<vector<E>> g;

    explicit Steiner(int _n) : n(_n), g(_n + 1) {
        assert(n >= 0);
    }

    void add(int u, int v, T w) {
        assert(1 <= u && u <= n && 1 <= v && v <= n && w >= 0);
        g[u].pb({v, w});
        g[v].pb({u, w});
    }

    optional<T> ask(const vector<int> &t) const {
        int k = t.size();
        assert(k <= 20);
        vector<char> vis(n + 1);
        for (int i = 0; i < k; ++i) {
            assert(1 <= t[i] && t[i] <= n && !vis[t[i]]);
            vis[t[i]] = true;
        }
        if (!k) return T{};
        int s = 1LL << k;
        T inf = numeric_limits<T>::max() / 4;
        vector<vector<T>> f(s, vector<T>(n + 1, inf));
        for (int i = 0; i < k; ++i) f[1LL << i][t[i]] = 0;
        using P = pair<T, int>;
        for (int m = 1; m < s; ++m) {
            for (int z = (m - 1) & m; z; z = (z - 1) & m) {
                int y = m ^ z;
                if (z >= y) continue;
                for (int v = 1; v <= n; ++v) {
                    if (f[z][v] == inf || f[y][v] == inf) continue;
                    if (f[z][v] > inf - f[y][v]) continue;
                    f[m][v] = min(f[m][v], f[z][v] + f[y][v]);
                }
            }
            priority_queue<P, vector<P>, greater<P>> q;
            for (int v = 1; v <= n; ++v)
                if (f[m][v] != inf) q.push({f[m][v], v});
            while (!q.empty()) {
                auto [d, u] = q.top();
                q.pop();
                if (d != f[m][u]) continue;
                for (auto e : g[u]) {
                    if (d > inf - e.w) continue;
                    T nd = d + e.w;
                    if (nd < f[m][e.v]) {
                        f[m][e.v] = nd;
                        q.push({nd, e.v});
                    }
                }
            }
        }
        T ans = inf;
        for (int v = 1; v <= n; ++v) ans = min(ans, f[s - 1][v]);
        if (ans == inf) return nullopt;
        return ans;
    }
};
