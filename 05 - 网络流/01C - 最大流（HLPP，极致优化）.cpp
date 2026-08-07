/*
用途：highest-label 预流推进的极低常数版，使用反向 BFS、gap、指针当前弧与周期重标号。
用法：接口与 01A / 01B 的 MaxFlow 一致；大图卡常时替换使用。
约定：点编号 0..n-1，Cap 必须是有符号整型；容量默认使用宏展开后的 int。
性能：点边下标必须保留 i32；改成宏展开后的 int 会增大边表并降低缓存命中率。
*/

using i32 = int32_t;

template <class Cap = int> struct MaxFlow {
    static constexpr Cap INF = numeric_limits<Cap>::max() / 4;

    struct Edge {
        i32 v, r;
        Cap f, c;
    };
    struct EdgeInfo {
        int u, v;
        Cap c, f;
    };

    i32 n, s = -1, t = -1, hi = 0, lm = 0;
    int wk = 0;
    vector<vector<Edge>> g;
    vector<array<i32, 2>> pos;
    vector<Cap> ex;
    vector<i32> h, cnt;
    vector<Edge*> cur;
    vector<vector<i32>> b;

    explicit MaxFlow(int n) : n(n), g(n) {
        assert(0 <= n && n <= numeric_limits<i32>::max());
    }

    int add(int u, int v, Cap c) {
        assert(0 <= u && u < n && 0 <= v && v < n && c >= 0);
        i32 x = g[u].size(), y = g[v].size() + (u == v);
        g[u].push_back({i32(v), y, 0, c});
        g[v].push_back({i32(u), x, 0, 0});
        pos.push_back({i32(u), x});
        return pos.size() - 1;
    }

    void add2(int u, int v, Cap c) {
        add(u, v, c), add(v, u, c);
    }

    void reset() {
        for (auto& a : g) {
            for (auto& e : a) e.c += e.f, e.f = 0;
        }
    }

    EdgeInfo askEdge(int id) const {
        int m = pos.size();
        assert(0 <= id && id < m);
        auto [u, i] = pos[id];
        const auto& e = g[u][i];
        return {u, e.v, e.c + e.f, e.f};
    }

    Cap askFlow(int id) const {
        return askEdge(id).f;
    }

    vector<EdgeInfo> askAll() const {
        vector<EdgeInfo> a;
        int m = pos.size();
        a.reserve(m);
        for (int i = 0; i < m; ++i) a.push_back(askEdge(i));
        return a;
    }

    void erase(int id) {
        int m = pos.size();
        assert(0 <= id && id < m);
        auto [u, i] = pos[id];
        auto& e = g[u][i];
        auto& r = g[e.v][e.r];
        e.f = e.c = r.f = r.c = 0;
    }

    void active(i32 u) {
        if (u == s || u == t || ex[u] <= 0 || h[u] >= lm) return;
        b[h[u]].push_back(u);
        hi = max(hi, h[u]);
    }

    void bfs() {
        lm = 2 * n + 1;
        h.assign(n, n + 1);
        cnt.assign(lm + 1, 0);
        cur.resize(n);
        b.assign(lm + 1, {});
        queue<i32> q;
        h[t] = 0, q.push(t);
        while (!q.empty()) {
            i32 u = q.front();
            q.pop();
            for (const auto& e : g[u]) {
                if (e.v == s || h[e.v] != n + 1 || g[e.v][e.r].c <= 0) continue;
                h[e.v] = h[u] + 1;
                q.push(e.v);
            }
        }
        h[s] = n;
        for (i32 u = 0; u < n; ++u) ++cnt[h[u]], cur[u] = g[u].data();
        hi = 0, wk = 0;
        for (i32 u = 0; u < n; ++u) active(u);
    }

    void gap(i32 k, i32 z) {
        for (i32 u = 0; u < n; ++u) {
            if (u == s || u == t || h[u] <= k || h[u] >= n + 1) continue;
            --cnt[h[u]];
            h[u] = n + 1;
            ++cnt[h[u]];
            cur[u] = g[u].data();
            if (u != z) active(u);
        }
    }

    void label(i32 u) {
        i32 oh = h[u], nh = lm;
        for (const auto& e : g[u]) {
            ++wk;
            if (e.c > 0) nh = min(nh, i32(h[e.v] + 1));
        }
        --cnt[oh];
        h[u] = nh;
        ++cnt[nh];
        cur[u] = g[u].data();
        if (oh < n && !cnt[oh]) gap(oh, u);
    }

    void push(i32 u, Edge& e) {
        Cap f = min(ex[u], e.c);
        if (f <= 0 || h[u] != h[e.v] + 1) return;
        bool z = ex[e.v] == 0;
        auto& r = g[e.v][e.r];
        e.f += f, e.c -= f;
        r.f -= f, r.c += f;
        ex[u] -= f, ex[e.v] += f;
        if (z) active(e.v);
    }

    void work(i32 u) {
        Edge* ed = g[u].data() + g[u].size();
        while (ex[u] > 0 && h[u] < lm) {
            if (cur[u] == ed) {
                label(u);
                ed = g[u].data() + g[u].size();
            } else if (cur[u]->c > 0 && h[u] == h[cur[u]->v] + 1) {
                push(u, *cur[u]);
            } else {
                ++cur[u], ++wk;
            }
        }
        active(u);
    }

    Cap flow(int _s, int _t) {
        assert(0 <= _s && _s < n && 0 <= _t && _t < n && _s != _t);
        s = _s, t = _t;
        ex.assign(n, Cap{});
        bfs();
        for (auto& e : g[s]) {
            if (e.v == s || e.c <= 0) continue;
            Cap f = e.c;
            auto& r = g[e.v][e.r];
            e.f += f, e.c = 0;
            r.f -= f, r.c += f;
            ex[s] -= f, ex[e.v] += f;
            active(e.v);
        }
        int m = pos.size();
        while (hi >= 0) {
            if (b[hi].empty()) {
                --hi;
                continue;
            }
            i32 u = b[hi].back();
            b[hi].pop_back();
            if (h[u] != hi || ex[u] <= 0) continue;
            work(u);
            if (wk > 4 * m + n) bfs();
        }
        return ex[t];
    }

    vector<char> askCut(int s) const {
        assert(0 <= s && s < n);
        vector<char> a(n);
        queue<i32> q;
        q.push(s), a[s] = true;
        while (!q.empty()) {
            i32 u = q.front();
            q.pop();
            for (const auto& e : g[u]) {
                if (e.c > 0 && !a[e.v]) a[e.v] = true, q.push(e.v);
            }
        }
        return a;
    }
};
