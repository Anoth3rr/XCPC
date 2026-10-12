namespace Graph {
    const int N = 403 * 2; //两倍点数
    typedef int T; //权值大小
    const T inf = numeric_limits<int>::max() >> 1;
    struct Q { int u, v; T w; } e[N][N];
    T lab[N];
    int n, m = 0, id, h, t, lk[N], sl[N], st[N], f[N], b[N][N], s[N], ed[N], q[N];
    vector<int> p[N];
#define dvd(x) (lab[x.u] + lab[x.v] - e[x.u][x.v].w * 2)
#define FOR(i, b) for (int i = 1; i <= (int)(b); i++)
#define ALL(x) (x).begin(), (x).end()
#define ms(x, i) memset(x + 1, i, m * sizeof x[0])
    void upd(int u, int v) {
        if (!sl[v] || dvd(e[u][v]) < dvd(e[sl[v]][v])) {
            sl[v] = u;
        }
    }
    void ss(int v) {
        sl[v] = 0;
        FOR(u, n) {
            if (e[u][v].w > 0 && st[u] != v && !s[st[u]]) {
                upd(u, v);
            }
        }
    }
    void ins(int u) {
        if (u <= n) { q[++t] = u; }
        else {
            for (int v : p[u]) ins(v);
        }
    }
    void mdf(int u, int w) {
        st[u] = w;
        if (u > n) {
            for (int v : p[u]) mdf(v, w);
        }
    }
    int gr(int u, int v) {
        v = find(ALL(p[u]), v) - p[u].begin();
        if (v & 1) {
            reverse(1 + ALL(p[u]));
            return (int)p[u].size() - v;
        }
        return v;
    }
    void stm(int u, int v) {
        lk[u] = e[u][v].v;
        if (u <= n) return;
        Q w = e[u][v];
        int x = b[u][w.u], y = gr(u, x);
        for (int i = 0; i < y; i++) {
            stm(p[u][i], p[u][i ^ 1]);
        }
        stm(x, v);
        rotate(p[u].begin(), y + ALL(p[u]));
    }
    void aug(int u, int v) {
        int w = st[lk[u]];
        stm(u, v);
        if (!w) return;
        stm(w, st[f[w]]), aug(st[f[w]], w);
    }
    int lca(int u, int v) {
        for (++id; u | v; swap(u, v)) {
            if (!u) continue;
            if (ed[u] == id) return u;
            ed[u] = id;
            if (u = st[lk[u]]) u = st[f[u]];
        }
        return 0;
    }
    void add(int u, int a, int v) {
        int x = n + 1, i, j;
        while (x <= m && st[x]) ++x;
        if (x > m) ++m;
        lab[x] = s[x] = st[x] = 0;
        lk[x] = lk[a];
        p[x].clear();
        p[x].push_back(a);
        for (i = u; i != a; i = st[f[j]]) {
            p[x].push_back(i);
            p[x].push_back(j = st[lk[i]]);
            ins(j);
        }
        reverse(1 + ALL(p[x]));
        for (i = v; i != a; i = st[f[j]]) { // 复制，只需改循环
            p[x].push_back(i);
            p[x].push_back(j = st[lk[i]]);
            ins(j);
        }
        mdf(x, x);
        FOR(i, m) {
            e[x][i].w = e[i][x].w = 0;
        }
        memset(b[x] + 1, 0, n * sizeof b[0][0]);
        for (int u : p[x]) {
            FOR(v, m) {
                if (!e[x][v].w || dvd(e[u][v]) < dvd(e[x][v])) {
                    e[x][v] = e[u][v], e[v][x] = e[v][u];
                }
            }
            FOR(v, n) {
                if (b[u][v]) { b[x][v] = u; }
            }
        }
        ss(x);
    }
    void ex(int u) {
        for (int x : p[u]) mdf(x, x);
        int a = b[u][e[u][f[u]].u], r = gr(u, a);
        for (int i = 0; i < r; i += 2) {
            int x = p[u][i], y = p[u][i + 1];
            f[x] = e[y][x].u;
            s[x] = 1;
            s[y] = sl[x] = 0;
            ss(y), ins(y);
        }
        s[a] = 1, f[a] = f[u];
        for (int i = r + 1; i < p[u].size(); i++) {
            s[p[u][i]] = -1;
            ss(p[u][i]);
        }
        st[u] = 0;
    }
    bool on(const Q &e) {
        int u = st[e.u], v = st[e.v];
        if (s[v] == -1) {
            f[v] = e.u, s[v] = 1;
            int a = st[lk[v]];
            sl[v] = sl[a] = s[a] = 0;
            ins(a);
        } else if (!s[v]) {
            int a = lca(u, v);
            if (!a) {
                return aug(u, v), aug(v, u), 1;
            } else {
                add(u, a, v);
            }
        }
        return 0;
    }
    bool bfs() {
        ms(s, -1), ms(sl, 0);
        h = 1, t = 0;
        FOR(i, m) {
            if (st[i] == i && !lk[i]) {
                f[i] = s[i] = 0;
                ins(i);
            }
        }
        if (h > t) return 0;
        while (1) {
            while (h <= t) {
                int u = q[h++];
                if (s[st[u]] == 1) continue;
                FOR(v, n) {
                    if (e[u][v].w > 0 && st[u] != st[v]) {
                        if (dvd(e[u][v])) upd(u, st[v]);
                        else if (on(e[u][v])) return 1;
                    }
                }
            }
            T x = inf;
            for (int i = n + 1; i <= m; i++) {
                if (st[i] == i && s[i] == 1) {
                    x = min(x, lab[i] >> 1);
                }
            }
            FOR(i, m) {
                if (st[i] == i && sl[i] && s[i] != 1) {
                    x = min(x, dvd(e[sl[i]][i]) >> s[i] + 1);
                }
            }
            FOR(i, n) {
                if (~s[st[i]]) {
                    if ((lab[i] += (s[st[i]] * 2 - 1) * x) <= 0) return 0;
                }
            }
            for (int i = n + 1; i <= m; i++) {
                if (st[i] == i && ~s[st[i]]) {
                    lab[i] += (2 - s[st[i]] * 4) * x;
                }
            }
            h = 1, t = 0;
            FOR(i, m) {
                if (st[i] == i && sl[i] && st[sl[i]] != i && !dvd(e[sl[i]][i]) && on(e[sl[i]][i])) {
                    return 1;
                }
            }
            for (int i = n + 1; i <= m; i++) {
                if (st[i] == i && s[i] == 1 && !lab[i]) ex(i);
            }
        }
        return 0;
    }
    template<typename TT> i64 work(int N, const vector<tuple<int, int, TT>> &edges) {
        ms(ed, 0), ms(lk, 0);
        n = m = N; id = 0;
        iota(st + 1, st + n + 1, 1);
        T wm = 0; i64 r = 0;
        FOR(i, n) FOR(j, n) {
            e[i][j] = {i, j, 0};
        }
        for (auto [u, v, w] : edges) {
            wm = max(wm, e[v][u].w = e[u][v].w = max(e[u][v].w, (T)w));
        }
        FOR(i, n) { p[i].clear(); }
        FOR(i, n) FOR(j, n) {
            b[i][j] = i * (i == j);
        }
        fill_n(lab + 1, n, wm);
        while (bfs()) {};
        FOR(i, n) if (lk[i]) {
            r += e[i][lk[i]].w;
        }
        return r / 2;
    }
    auto match() {
        vector<array<int, 2>> ans;
        FOR(i, n) if (lk[i]) {
            ans.push_back({i, lk[i]});
        }
        return ans;
    }
} // namespace Graph
using Graph::work, Graph::match;

signed main() {
    int n, m;
    cin >> n >> m;
    vector<tuple<int, int, i64>> ver(m);
    for (auto &[u, v, w] : ver) {
        cin >> u >> v >> w;
    }
    cout << work(n, ver) << "\n";
    auto ans = match();
}
