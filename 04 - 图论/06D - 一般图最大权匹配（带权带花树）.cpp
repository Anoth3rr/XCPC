/*
用途：一般无向图最大权匹配（允许点不匹配），Edmonds 带权带花树。
约定：原点编号 1..n，n<N，只考虑正权边；重边取最大权。复杂度 O(n^3)。
若要求完美匹配且有负权，可先给每条边统一加足够大的常数，再检查匹配边数为 n/2。
点、花与矩阵下标使用 int。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/

namespace Blossom {
using W = int;
constexpr int N = 405;
constexpr int V = N * 2 + 5;
constexpr W INF = numeric_limits<W>::max() / 4;

struct Edge {
    int u = 0, v = 0;
    W w = 0;
};
struct Result {
    W w = 0;
    vector<array<int, 2>> es;
};

inline Edge e[V][V];
inline W lab[V];
inline int n, m, tim, ql, qr;
inline int lk[V], sl[V], st[V], fa[V], bel[V][V], typ[V], vis[V], que[V];
inline vector<int> cyc[V];

inline W slack(const Edge& x) { return lab[x.u] + lab[x.v] - 2 * x.w; }

void upd(int u, int v) {
    if (!sl[v] || slack(e[u][v]) < slack(e[sl[v]][v])) sl[v] = u;
}

void slack(int v) {
    sl[v] = 0;
    for (int u = 1; u <= n; ++u)
        if (e[u][v].w > 0 && st[u] != v && typ[st[u]] == 0) upd(u, v);
}

void push(int u) {
    if (u <= n) que[++qr] = u;
    else for (int v : cyc[u]) push(v);
}

void setSt(int u, int v) {
    st[u] = v;
    if (u > n) for (int x : cyc[u]) setSt(x, v);
}

int rot(int u, int v) {
    int k = find(cyc[u].begin(), cyc[u].end(), v) - cyc[u].begin();
    if (k & 1) {
        reverse(cyc[u].begin() + 1, cyc[u].end());
        return cyc[u].size() - k;
    }
    return k;
}

void match(int u, int v) {
    lk[u] = e[u][v].v;
    if (u <= n) return;
    Edge w = e[u][v];
    int x = bel[u][w.u], y = rot(u, x);
    for (int i = 0; i < y; ++i) match(cyc[u][i], cyc[u][i ^ 1]);
    match(x, v);
    rotate(cyc[u].begin(), cyc[u].begin() + y, cyc[u].end());
}

void augment(int u, int v) {
    int w = st[lk[u]];
    match(u, v);
    if (!w) return;
    match(w, st[fa[w]]);
    augment(st[fa[w]], w);
}

int lca(int u, int v) {
    for (++tim; u || v; swap(u, v)) {
        if (!u) continue;
        if (vis[u] == tim) return u;
        vis[u] = tim;
        if ((u = st[lk[u]])) u = st[fa[u]];
    }
    return 0;
}

void add(int u, int a, int v) {
    int x = n + 1;
    while (x <= m && st[x]) ++x;
    if (x > m) ++m;
    assert(m < V);
    lab[x] = 0, typ[x] = 0, st[x] = x, lk[x] = lk[a];
    cyc[x].clear();
    cyc[x].push_back(a);
    int i, j = 0;
    for (i = u; i != a; i = st[fa[j]]) {
        cyc[x].push_back(i);
        cyc[x].push_back(j = st[lk[i]]);
        push(j);
    }
    reverse(cyc[x].begin() + 1, cyc[x].end());
    for (i = v; i != a; i = st[fa[j]]) {
        cyc[x].push_back(i);
        cyc[x].push_back(j = st[lk[i]]);
        push(j);
    }
    setSt(x, x);
    for (i = 1; i <= m; ++i) e[x][i] = e[i][x] = {};
    fill(bel[x], bel[x] + n + 1, 0);
    for (int z : cyc[x]) {
        for (int y = 1; y <= m; ++y) {
            if (!e[x][y].w || slack(e[z][y]) < slack(e[x][y])) e[x][y] = e[z][y], e[y][x] = e[y][z];
        }
        for (int y = 1; y <= n; ++y) if (bel[z][y]) bel[x][y] = z;
    }
    slack(x);
}

void expand(int u) {
    for (int x : cyc[u]) setSt(x, x);
    int a = bel[u][e[u][fa[u]].u], r = rot(u, a);
    for (int i = 0; i < r; i += 2) {
        int x = cyc[u][i], y = cyc[u][i + 1];
        fa[x] = e[y][x].u;
        typ[x] = 1;
        typ[y] = sl[x] = 0;
        slack(y), push(y);
    }
    typ[a] = 1, fa[a] = fa[u];
    for (int i = r + 1; i < cyc[u].size(); ++i) typ[cyc[u][i]] = -1, slack(cyc[u][i]);
    st[u] = 0;
}

bool onEdge(const Edge& x) {
    int u = st[x.u], v = st[x.v];
    if (typ[v] == -1) {
        fa[v] = x.u, typ[v] = 1;
        int a = st[lk[v]];
        sl[v] = sl[a] = typ[a] = 0;
        push(a);
    } else if (typ[v] == 0) {
        int a = lca(u, v);
        if (!a) {
            augment(u, v), augment(v, u);
            return true;
        }
        add(u, a, v);
    }
    return false;
}

bool bfs() {
    fill(typ + 1, typ + m + 1, -1);
    fill(sl + 1, sl + m + 1, 0);
    ql = 1, qr = 0;
    for (int i = 1; i <= m; ++i) if (st[i] == i && !lk[i]) {
        fa[i] = typ[i] = 0;
        push(i);
    }
    if (ql > qr) return false;
    while (true) {
        while (ql <= qr) {
            int u = que[ql++];
            if (typ[st[u]] == 1) continue;
            for (int v = 1; v <= n; ++v) if (e[u][v].w > 0 && st[u] != st[v]) {
                if (slack(e[u][v])) upd(u, st[v]);
                else if (onEdge(e[u][v])) return true;
            }
        }

        W d = INF;
        for (int i = n + 1; i <= m; ++i) if (st[i] == i && typ[i] == 1) d = min(d, lab[i] / 2);
        for (int i = 1; i <= m; ++i) if (st[i] == i && sl[i] && typ[i] != 1)
            d = min(d, slack(e[sl[i]][i]) / (typ[i] == 0 ? 2 : 1));
        if (d == INF) return false;

        for (int i = 1; i <= n; ++i) if (typ[st[i]] != -1) {
            lab[i] += (2 * typ[st[i]] - 1) * d;
            if (lab[i] <= 0) return false;
        }
        for (int i = n + 1; i <= m; ++i) if (st[i] == i && typ[i] != -1)
            lab[i] += (2 - 4 * typ[i]) * d;

        ql = 1, qr = 0;
        for (int i = 1; i <= m; ++i)
            if (st[i] == i && sl[i] && st[sl[i]] != i && !slack(e[sl[i]][i]) && onEdge(e[sl[i]][i])) return true;
        for (int i = n + 1; i <= m; ++i) if (st[i] == i && typ[i] == 1 && !lab[i]) expand(i);
    }
}

Result solve(int _n, const vector<tuple<int, int, W>>& es) {
    assert(0 <= _n && _n < N);
    n = m = _n, tim = 0;
    int lim = min<int>(V, 2 * n + 5);
    fill(lk, lk + lim, 0), fill(st, st + lim, 0), fill(vis, vis + lim, 0);
    for (int i = 0; i < lim; ++i) {
        fill(e[i], e[i] + lim, Edge{});
        fill(bel[i], bel[i] + lim, 0);
        cyc[i].clear();
    }
    for (int i = 1; i <= n; ++i) st[i] = i;
    W mx = 0;
    for (int i = 1; i <= n; ++i) for (int j = 1; j <= n; ++j) e[i][j] = {i, j, 0};
    for (auto [u, v, w] : es) {
        assert(1 <= u && u <= n && 1 <= v && v <= n);
        if (u == v || w <= 0) continue;
        if (w > e[u][v].w) e[u][v] = {u, v, w}, e[v][u] = {v, u, w};
        mx = max(mx, w);
    }
    for (int i = 1; i <= n; ++i) {
        lab[i] = mx;
        for (int j = 1; j <= n; ++j) if (i == j) bel[i][j] = i;
    }
    while (bfs()) {}
    Result r;
    for (int i = 1; i <= n; ++i) if (lk[i] && i < lk[i]) {
        r.w += e[i][lk[i]].w;
        r.es.push_back({i, lk[i]});
    }
    return r;
}
}  // namespace Blossom
