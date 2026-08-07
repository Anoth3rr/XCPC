/*
用途：无向图找割点、点双连通分量，并建立圆方树（block-cut tree）。
约定：点编号 1..n；支持重边；默认不放自环。原图非连通时 blockCutTree 为森林。
结果中原点仍为 1..n，第 i 个点双连通分量对应圆方树节点 n + i + 1。
复杂度：O(n + m)。
*/
struct VBCC {
    struct Edge {
        int u, v;
    };

    struct Result {
        vector<char> cut;
        vector<vector<int>> cc;
        vector<vector<int>> tr;
    };

    int n;
    vector<Edge> e;
    vector<vector<pair<int, int>>> g;

    explicit VBCC(int n) : n(n), g(n + 1) {}

    int add(int u, int v) {
        int id = e.size();
        e.push_back({u, v});
        g[u].push_back({v, id});
        g[v].push_back({u, id});
        return id;
    }

    Result solve() const {
        vector<int> dfn(n + 1), low(n + 1), stk;
        vector<char> cut(n + 1, false);
        vector<vector<int>> cc;
        int tim = 0;

        auto dfs = [&](auto&& go, int u, int pe) -> void {
            dfn[u] = low[u] = ++tim;
            int son = 0;
            for (auto [v, id] : g[u]) {
                if (id == pe) continue;
                if (!dfn[v]) {
                    ++son;
                    stk.push_back(id);
                    go(go, v, id);
                    low[u] = min(low[u], low[v]);

                    if (low[v] >= dfn[u]) {
                        if (pe != -1 || son > 1) cut[u] = true;
                        vector<int> cur;
                        while (true) {
                            int eid = stk.back();
                            stk.pop_back();
                            cur.push_back(e[eid].u);
                            cur.push_back(e[eid].v);
                            if (eid == id) break;
                        }
                        sort(cur.begin(), cur.end());
                        cur.erase(unique(cur.begin(), cur.end()), cur.end());
                        cc.push_back(move(cur));
                    }
                } else if (dfn[v] < dfn[u]) {
                    stk.push_back(id);
                    low[u] = min(low[u], dfn[v]);
                }
            }
        };

        for (int u = 1; u <= n; ++u) {
            if (dfn[u]) continue;
            if (g[u].empty()) {
                dfn[u] = low[u] = ++tim;
                cc.push_back({u});
            } else {
                dfs(dfs, u, -1);
            }
        }

        vector<vector<int>> tr(n + cc.size() + 1);
        for (int id = 0; id < cc.size(); ++id) {
            int b = n + id + 1;
            for (int u : cc[id]) {
                tr[u].push_back(b);
                tr[b].push_back(u);
            }
        }
        return {move(cut), move(cc), move(tr)};
    }
};
