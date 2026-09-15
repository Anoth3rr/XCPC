struct CenDec {
    struct Edge {
        int to;
        int w;
    };

    int n;
    vector<vector<Edge>> e;
    vector<int> siz, cfa, dep;
    vector<char> ban;

    CenDec(int n) : n(n), e(n + 1), siz(n + 1), cfa(n + 1), dep(n + 1), ban(n + 1) {}

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

    template <class F> void divide(int st, int fa, int d, F &f) {
        int tot = dfsSz(st, 0);
        int c = cen(st, 0, tot);
        cfa[c] = fa;
        dep[c] = d;
        ban[c] = true;
        f(c, fa, d);

        for (auto [v, w] : e[c]) {
            if (!ban[v]) divide(v, c, d + 1, f);
        }
    }

    template <class F> void build(F f, int rt = 1) {
        fill(siz.begin(), siz.end(), 0);
        fill(cfa.begin(), cfa.end(), 0);
        fill(dep.begin(), dep.end(), 0);
        fill(ban.begin(), ban.end(), 0);
        divide(rt, 0, 0, f);
    }
};