/*
用途：有向图以 root 为根的最小树形图（每个非根点恰有一条入边，且均从根可达）。
约定：点编号 0..n-1，允许负权和重边；返回 nullopt 表示不存在。仅返回最小权值。
复杂度：O(nm)。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/

template <class T = int> struct DMST {
    struct Edge {
        int u, v;
        T w;
    };

    int n;
    vector<Edge> e;
    explicit DMST(int n) : n(n) { assert(n >= 0); }

    void add(int u, int v, T w) {
        assert(0 <= u && u < n && 0 <= v && v < n);
        e.push_back({u, v, w});
    }

    optional<T> solve(int rt) const {
        assert(0 <= rt && rt < n);
        const T inf = numeric_limits<T>::max() / 4;
        int z = n, r = rt;
        vector<Edge> a = e;
        T ans{};
        while (true) {
            vector<T> in(z, inf);
            vector<int> pre(z, -1);
            for (auto [u, v, w] : a) {
                if (u != v && w < in[v]) in[v] = w, pre[v] = u;
            }
            in[r] = T{};
            for (int i = 0; i < z; ++i) {
                if (in[i] == inf) return nullopt;
                ans += in[i];
            }

            int cnt = 0;
            vector<int> id(z, -1), vis(z, -1);
            for (int i = 0; i < z; ++i) {
                int v = i;
                while (vis[v] != i && id[v] == -1 && v != r) vis[v] = i, v = pre[v];
                if (v == r || id[v] != -1) continue;
                for (int u = pre[v]; u != v; u = pre[u]) id[u] = cnt;
                id[v] = cnt++;
            }
            if (!cnt) break;
            for (int i = 0; i < z; ++i) if (id[i] == -1) id[i] = cnt++;

            vector<Edge> b;
            b.reserve(a.size());
            for (auto [u, v, w] : a) {
                int x = id[u], y = id[v];
                if (x != y) b.push_back({x, y, w - in[v]});
            }
            r = id[r], z = cnt, a.swap(b);
        }
        return ans;
    }
};
