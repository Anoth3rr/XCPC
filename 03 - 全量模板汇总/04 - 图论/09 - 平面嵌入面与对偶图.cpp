/*
用途：已知连通直线平面嵌入（无交叉）时，枚举面并建立带权对偶图。
约定：每条无向边给一个权；对偶边连接其左右两面。桥的左右面相同，不加入对偶自环。
复杂度：O(m log m)，主要为每点按极角排序。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/

using i128 = __int128_t;

template <class T = int, class W = int, class D = i128> struct PlanarDual {
    struct P { T x, y; };
    struct E { int u, v; W w; };

    vector<P> p;
    vector<E> e;
    vector<array<int, 2>> he;
    vector<int> rev, nxt, bel;
    vector<vector<int>> g;
    vector<D> ar;
    int out = -1;

    PlanarDual() = default;
    explicit PlanarDual(vector<P> p) : p(move(p)), g(this->p.size()) {}

    void add(int u, int v, W w = 1) {
        int id = e.size(), a = he.size();
        e.push_back({u, v, w});
        he.push_back({u, v});
        he.push_back({v, u});
        rev.push_back(a + 1);
        rev.push_back(a);
        g[u].push_back(a);
        g[v].push_back(a + 1);
    }

    D cr(int a, int b, int c) const {
        return D(p[b].x - p[a].x) * (p[c].y - p[a].y) - D(p[b].y - p[a].y) * (p[c].x - p[a].x);
    }

    void build() {
        int m = he.size();
        vector<int> pos(m);
        auto hf = [&](T x, T y) { return y < 0 || (y == 0 && x < 0); };
        for (int u = 0; u < g.size(); ++u) {
            sort(g[u].begin(), g[u].end(), [&](int a, int b) {
                auto [ua, x] = he[a];
                auto [ub, y] = he[b];
                T ax = p[x].x - p[u].x, ay = p[x].y - p[u].y;
                T bx = p[y].x - p[u].x, by = p[y].y - p[u].y;
                if (hf(ax, ay) != hf(bx, by)) return hf(ax, ay) < hf(bx, by);
                D z = D(ax) * by - D(ay) * bx;
                if (z) return z > 0;
                return D(ax) * ax + D(ay) * ay < D(bx) * bx + D(by) * by;
            });
            for (int i = 0; i < g[u].size(); ++i) pos[g[u][i]] = i;
        }

        nxt.assign(m, -1);
        for (int h = 0; h < m; ++h) {
            int v = he[h][1], k = pos[rev[h]], d = g[v].size();
            nxt[h] = g[v][(k + d - 1) % d];
        }

        bel.assign(m, -1);
        ar.clear();
        for (int s = 0; s < m; ++s) if (bel[s] < 0) {
            int f = ar.size(), h = s;
            D a = 0;
            do {
                bel[h] = f;
                auto [u, v] = he[h];
                a += D(p[u].x) * p[v].y - D(p[u].y) * p[v].x;
                h = nxt[h];
            } while (h != s);
            ar.push_back(a);
        }
        out = min_element(ar.begin(), ar.end()) - ar.begin();
    }

    vector<vector<pair<int, W>>> ask() const {
        vector<vector<pair<int, W>>> d(ar.size());
        for (int i = 0; i < e.size(); ++i) {
            int a = bel[i << 1], b = bel[i << 1 | 1];
            if (a == b) continue;
            d[a].push_back({b, e[i].w});
            d[b].push_back({a, e[i].w});
        }
        return d;
    }
};
