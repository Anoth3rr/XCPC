/*
用途：连续最短增广路最小费用最大流，支持初始负费用边。
约定：点编号 0..n-1，容量非负；不存在从源点可达的负费用环。
flow 不自动 reset，若需重复求解请先 reset。返回 nullopt 表示检测到可达负环。
复杂度：每次增广 Dijkstra O(m log n)，总次数至多为增广次数。
*/

template <class Cap = int, class C = int> struct MCMF {
    static constexpr Cap FI = numeric_limits<Cap>::max() / 4;
    static constexpr C CI = numeric_limits<C>::max() / 4;

    struct Edge {
        int v, r;
        Cap c, oc;
        C w;
    };

    struct EdgeInfo {
        int u, v;
        Cap c, f;
        C w;
    };

    struct Result {
        Cap f;
        C c;
    };

    int n;
    vector<vector<Edge>> g;
    vector<pair<int, int>> pos;

    explicit MCMF(int n) : n(n), g(n) {}

    int add(int u, int v, Cap c, C w) {
        assert(0 <= u && u < n && 0 <= v && v < n && c >= 0);
        int id = g[u].size();
        int r = g[v].size() + (u == v);
        g[u].push_back({v, r, c, c, w});
        g[v].push_back({u, id, 0, 0, -w});
        pos.push_back({u, id});
        return pos.size() - 1;
    }

    void reset() {
        for (auto& a : g) {
            for (Edge& e : a) e.c = e.oc;
        }
    }

    EdgeInfo askEdge(int id) const {
        assert(0 <= id && id < pos.size());
        auto [u, i] = pos[id];
        const Edge& e = g[u][i];
        return {u, e.v, e.oc, e.oc - e.c, e.w};
    }

    vector<EdgeInfo> askAll() const {
        vector<EdgeInfo> res;
        res.reserve(pos.size());
        for (int id = 0; id < pos.size(); ++id) res.push_back(askEdge(id));
        return res;
    }

    optional<Result> flow(int s, int t, Cap lim = FI) {
        static_assert(std::is_signed_v<C>, "Cost must be signed because reverse edges negate it.");
        assert(0 <= s && s < n && 0 <= t && t < n && s != t && lim >= 0);

        vector<C> h;
        if (!initPot(s, h)) return nullopt;

        Cap f = 0;
        C c = 0;
        while (f < lim) {
            vector<C> dis(n, CI);
            vector<pair<int, int>> fa(n, {-1, -1});
            priority_queue<pair<C, int>, vector<pair<C, int>>, greater<pair<C, int>>> pq;
            dis[s] = 0;
            pq.push({0, s});

            while (!pq.empty()) {
                auto [du, u] = pq.top();
                pq.pop();
                if (du != dis[u]) continue;
                for (int i = 0; i < g[u].size(); ++i) {
                    const Edge& e = g[u][i];
                    if (e.c <= 0) continue;
                    C rc = e.w + h[u] - h[e.v];
                    if (dis[e.v] > du + rc) {
                        dis[e.v] = du + rc;
                        fa[e.v] = {u, i};
                        pq.push({dis[e.v], e.v});
                    }
                }
            }
            if (fa[t].first == -1) break;

            for (int u = 0; u < n; ++u) {
                if (dis[u] != CI) h[u] += dis[u];
            }

            Cap aug = lim - f;
            C pc = 0;
            for (int u = t; u != s;) {
                auto [pre, i] = fa[u];
                aug = min(aug, g[pre][i].c);
                pc += g[pre][i].w;
                u = pre;
            }
            for (int u = t; u != s;) {
                auto [pre, i] = fa[u];
                Edge& e = g[pre][i];
                e.c -= aug;
                g[e.v][e.r].c += aug;
                u = pre;
            }
            f += aug;
            c += static_cast<C>(aug) * pc;
        }
        return Result{f, c};
    }

  private:
    bool initPot(int s, vector<C>& h) const {
        vector<C> dis(n, CI);
        vector<int> pec(n);
        vector<char> inq(n, false);
        queue<int> q;
        dis[s] = 0;
        q.push(s);
        inq[s] = true;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            inq[u] = false;
            for (const Edge& e : g[u]) {
                if (e.c <= 0 || dis[e.v] <= dis[u] + e.w) continue;
                dis[e.v] = dis[u] + e.w;
                pec[e.v] = pec[u] + 1;
                if (pec[e.v] >= n) return false;
                if (!inq[e.v]) {
                    inq[e.v] = true;
                    q.push(e.v);
                }
            }
        }

        h.assign(n, 0);
        for (int u = 0; u < n; ++u) {
            if (dis[u] != CI) h[u] = dis[u];
        }
        return true;
    }
};
