/*
用途：正权图的最短路、严格次短路及其条数。
约定：点编号 1..n；边权必须严格为正。计数默认用 int，取模时传入 MInt。
接口：solve(s) 返回 dis[u][0/1]、cnt[u][0/1]；INF 表示不存在。
复杂度：O((n+m) log n)。计数的是对应长度的不同有向边序列；正权保证同距离计数已在弹堆前汇总。
*/
template <class T = int, class Z = int> struct TwoDij {
    static constexpr T INF = numeric_limits<T>::max() / 4;

    struct Edge {
        int v;
        T w;
    };
    struct Res {
        vector<array<T, 2>> dis;
        vector<array<Z, 2>> cnt;
    };

    int n;
    vector<vector<Edge>> g;

    explicit TwoDij(int n) : n(n), g(n + 1) {}

    void add(int u, int v, T w) {
        assert(w > T{} && w < INF);
        g[u].push_back({v, w});
    }

    void add2(int u, int v, T w) {
        add(u, v, w);
        add(v, u, w);
    }

    Res solve(int s) const {
        vector<array<T, 2>> dis(n + 1, {INF, INF});
        vector<array<Z, 2>> cnt(n + 1);
        vector<array<char, 2>> vis(n + 1);
        using S = tuple<T, int, int>;
        priority_queue<S, vector<S>, greater<S>> q;
        dis[s][0] = T{};
        cnt[s][0] = Z(1);
        q.emplace(T{}, s, 0);

        while (!q.empty()) {
            auto [d, u, k] = q.top();
            q.pop();
            if (d != dis[u][k] || vis[u][k]) continue;
            vis[u][k] = true;
            for (auto [v, w] : g[u]) {
                T nd = d + w;
                if (nd < dis[v][0]) {
                    dis[v][1] = dis[v][0];
                    cnt[v][1] = cnt[v][0];
                    dis[v][0] = nd;
                    cnt[v][0] = cnt[u][k];
                    q.emplace(dis[v][0], v, 0);
                    if (dis[v][1] != INF) q.emplace(dis[v][1], v, 1);
                } else if (nd == dis[v][0]) {
                    cnt[v][0] += cnt[u][k];
                } else if (nd < dis[v][1]) {
                    dis[v][1] = nd;
                    cnt[v][1] = cnt[u][k];
                    q.emplace(nd, v, 1);
                } else if (nd == dis[v][1]) {
                    cnt[v][1] += cnt[u][k];
                }
            }
        }
        return {move(dis), move(cnt)};
    }
};
