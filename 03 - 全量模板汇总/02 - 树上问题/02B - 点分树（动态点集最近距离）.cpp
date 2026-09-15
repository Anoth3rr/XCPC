struct CenTree {
    struct Edge {
        int to;
        int w;
    };

    struct PathInfo {
        int cen;
        int dis;
    };

    int n;
    vector<vector<Edge>> e;
    vector<int> siz, cfa;
    vector<char> ban, on;
    vector<vector<PathInfo>> pth;
    vector<multiset<int>> q;

    explicit CenTree(int n) : n(n), e(n + 1), siz(n + 1), cfa(n + 1), ban(n + 1), on(n + 1), pth(n + 1), q(n + 1) {}

    void add(int u, int v, int w = 1) {
        e[u].push_back({v, w});
        e[v].push_back({u, w});
    }

    int dfsSz(int u, int fa) {
        siz[u] = 1;
        for (auto [v, w] : e[u]) {
            if (v == fa || ban[v]) continue;
            siz[u] += dfsSz(v, u);
        }
        return siz[u];
    }

    int cen(int u, int fa, int tot) {
        for (auto [v, w] : e[u]) {
            if (v == fa || ban[v]) continue;
            if (siz[v] > tot / 2) return cen(v, u, tot);
        }
        return u;
    }

    void dfsDis(int u, int fa, int dis, int cen) {
        pth[u].push_back({cen, dis});
        for (auto [v, w] : e[u]) {
            if (v == fa || ban[v]) continue;
            dfsDis(v, u, dis + w, cen);
        }
    }

    void build(int st, int fa) {
        int tot = dfsSz(st, 0);
        int cen = this->cen(st, 0, tot);
        cfa[cen] = fa;
        ban[cen] = true;
        dfsDis(cen, 0, 0, cen);

        for (auto [v, w] : e[cen]) {
            if (!ban[v]) build(v, cen);
        }
    }

    void build(int rt = 1) {
        fill(siz.begin(), siz.end(), 0);
        fill(cfa.begin(), cfa.end(), 0);
        fill(ban.begin(), ban.end(), false);
        fill(on.begin(), on.end(), false);
        for (int u = 1; u <= n; ++u) {
            pth[u].clear();
            q[u].clear();
        }
        build(rt, 0);
    }

    bool modify(int u, bool _on) {
        if ((bool)on[u] == _on) return false;
        on[u] = _on;
        for (auto [cen, dis] : pth[u]) {
            if (_on) {
                q[cen].insert(dis);
            } else {
                auto it = q[cen].find(dis);
                assert(it != q[cen].end());
                q[cen].erase(it);
            }
        }
        return true;
    }

    bool add(int u) {
        return modify(u, true);
    }

    bool del(int u) {
        return modify(u, false);
    }

    void clear() {
        fill(on.begin(), on.end(), false);
        for (int u = 1; u <= n; ++u) q[u].clear();
    }

    optional<int> askMin(int u) const {
        optional<int> ans;
        for (auto [cen, dis] : pth[u]) {
            if (q[cen].empty()) continue;
            int cur = dis + *q[cen].begin();
            if (!ans || cur < *ans) ans = cur;
        }
        return ans;
    }

    int ask(int u) const {
        auto ans = askMin(u);
        return ans ? *ans : -1;
    }
};