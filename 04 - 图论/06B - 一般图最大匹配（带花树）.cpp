/*
用途：Edmonds 带花树求一般无向图最大基数匹配。
约定：点编号 1..n；支持重边，默认不放自环。mat[u] 为匹配点，0 表示未匹配。
复杂度：O(n^3)，适合中等规模图。
*/
struct Blossom {
    struct Result {
        int cnt;
        vector<int> mat;
    };

    int n;
    vector<vector<int>> g;

    explicit Blossom(int n) : n(n), g(n + 1) {}

    void add(int u, int v) {
        assert(u != v);
        g[u].push_back(v);
        g[v].push_back(u);
    }

    Result solve() const {
        vector<int> mat(n + 1);
        vector<int> fa(n + 1), bas(n + 1);
        vector<char> use(n + 1), in(n + 1);

        auto lca = [&](int a, int b) -> int {
            vector<char> vis(n + 1, false);
            while (true) {
                a = bas[a];
                vis[a] = true;
                if (mat[a] == 0) break;
                a = fa[mat[a]];
            }
            while (true) {
                b = bas[b];
                if (vis[b]) return b;
                b = fa[mat[b]];
            }
        };

        auto tag = [&](int u, int rt, int son) -> void {
            while (bas[u] != rt) {
                in[bas[u]] = true;
                in[bas[mat[u]]] = true;
                fa[u] = son;
                son = mat[u];
                u = fa[mat[u]];
            }
        };

        auto bfs = [&](int s) -> int {
            fill(use.begin(), use.end(), false);
            fill(fa.begin(), fa.end(), 0);
            iota(bas.begin(), bas.end(), 0);
            queue<int> q;
            q.push(s);
            use[s] = true;

            while (!q.empty()) {
                int u = q.front();
                q.pop();
                for (int v : g[u]) {
                    if (bas[u] == bas[v] || mat[u] == v) continue;

                    if (v == s || (mat[v] != 0 && fa[mat[v]] != 0)) {
                        int rt = lca(u, v);
                        fill(in.begin(), in.end(), false);
                        tag(u, rt, v);
                        tag(v, rt, u);
                        for (int x = 1; x <= n; ++x) {
                            if (!in[bas[x]]) continue;
                            bas[x] = rt;
                            if (!use[x]) {
                                use[x] = true;
                                q.push(x);
                            }
                        }
                    } else if (fa[v] == 0) {
                        fa[v] = u;
                        if (mat[v] == 0) return v;
                        v = mat[v];
                        use[v] = true;
                        q.push(v);
                    }
                }
            }
            return 0;
        };

        for (int s = 1; s <= n; ++s) {
            if (mat[s] != 0) continue;
            int t = bfs(s);
            while (t != 0) {
                int pre = fa[t];
                int nxt = mat[pre];
                mat[t] = pre;
                mat[pre] = t;
                t = nxt;
            }
        }

        int cnt = 0;
        for (int u = 1; u <= n; ++u) {
            if (mat[u] > u) ++cnt;
        }
        return {cnt, move(mat)};
    }
};
