/*
用途：最高标号预流推进（HLPP）最大流，含反向 BFS 初始标号、当前弧与 gap 优化。
适合点边较多、Dinic 分层增广偏慢的图；小图和建模题通常优先更短的 01A Dinic。
约定：点号/边下标使用 int，容量由 Cap 适配；点编号 0..n-1。
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
    vector<Cap> ex;
    vector<int> h, cnt, cur;
    vector<vector<int>> b;
    int s = -1, t = -1, hi = 0, lmh = 0;

    explicit MaxFlow(int n) : n(n), g(n) { assert(n >= 0); }

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
        add(u, v, c), add(v, u, c);
    }

    void reset() {
        for (auto& a : g) for (auto& e : a) e.c = e.oc;
    }

    EdgeInfo askEdge(int id) const {
        assert(0 <= id && id < pos.size());
        auto [u, i] = pos[id];
        const auto& e = g[u][i];
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

    void active(int u) {
        if (u == s || u == t || ex[u] <= 0 || h[u] >= lmh) return;
        b[h[u]].push_back(u);
        hi = max(hi, h[u]);
    }

    void label() {
        lmh = 2 * n + 1;
        h.assign(n, n + 1);  // 不可达汇点的点仍可沿反向边把预流退回源点。
        queue<int> q;
        h[t] = 0, q.push(t);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (const auto& e : g[u]) {
                const auto& r = g[e.v][e.r];
                if (r.c <= 0 || h[e.v] != n + 1) continue;
                h[e.v] = h[u] + 1;
                q.push(e.v);
            }
        }
        h[s] = n;
        cnt.assign(lmh + 1, 0);
        for (int x : h) ++cnt[x];
        cur.assign(n, 0);
        b.assign(lmh + 1, {});
        hi = 0;
        for (int u = 0; u < n; ++u) active(u);
    }

    void push(int u, Edge& e) {
        Cap x = min(ex[u], e.c);
        if (x <= 0 || h[u] != h[e.v] + 1) return;
        bool z = ex[e.v] == 0;
        e.c -= x;
        g[e.v][e.r].c += x;
        ex[u] -= x, ex[e.v] += x;
        if (z) active(e.v);
    }

    void relabel(int u) {
        int old = h[u], nh = lmh;
        for (const auto& e : g[u]) if (e.c > 0) nh = min(nh, min<int>(lmh, h[e.v] + 1));
        --cnt[old];
        h[u] = nh;
        ++cnt[nh];
        cur[u] = 0;

        if (old < n && cnt[old] == 0) {
            for (int v = 0; v < n; ++v) {
                if (v == s || v == t || h[v] <= old || h[v] >= n) continue;
                --cnt[h[v]];
                h[v] = n + 1;
                ++cnt[h[v]];
                active(v);
            }
        }
    }

    void push(int u) {
        while (ex[u] > 0 && h[u] < lmh) {
            if (cur[u] == g[u].size()) {
                relabel(u);
                continue;
            }
            Edge& e = g[u][cur[u]];
            if (e.c > 0 && h[u] == h[e.v] + 1) push(u, e);
            else ++cur[u];
        }
        active(u);
    }

    Cap flow(int _s, int _t) {
        assert(0 <= _s && _s < n && 0 <= _t && _t < n && _s != _t);
        s = _s, t = _t;
        ex.assign(n, Cap{});
        label();
        for (auto& e : g[s]) {
            if (e.v == s || e.c <= 0) continue;
            Cap x = e.c;
            e.c = 0;
            g[e.v][e.r].c += x;
            ex[s] -= x, ex[e.v] += x;
            active(e.v);
        }
        while (hi >= 0) {
            if (b[hi].empty()) {
                --hi;
                continue;
            }
            int u = b[hi].back();
            b[hi].pop_back();
            if (h[u] != hi || ex[u] <= 0) continue;
            push(u);
        }
        return ex[t];
    }

    vector<char> askCut(int s) const {
        vector<char> a(n);
        queue<int> q;
        q.push(s), a[s] = true;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (const auto& e : g[u]) if (e.c > 0 && !a[e.v]) a[e.v] = true, q.push(e.v);
        }
        return a;
    }
};
