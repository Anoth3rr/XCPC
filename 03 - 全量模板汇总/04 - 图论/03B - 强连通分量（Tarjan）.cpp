/*
用途：有向图强连通分量、缩点 DAG。
约定：点编号 1..n，add 添加有向边。bel 编号为 0..cc-1，
编号顺序不承诺拓扑性质；dag 是去重后的缩点图。
复杂度：O(n + m)。
*/
struct SCC {
    struct Result {
        int cc;
        vector<int> bel;
        vector<vector<int>> scc;
        vector<vector<int>> dag;
    };

    int n;
    vector<vector<int>> g;

    explicit SCC(int n) : n(n), g(n + 1) {}

    void add(int u, int v) {
        g[u].push_back(v);
    }

    Result solve() const {
        vector<int> dfn(n + 1), low(n + 1), stk, bel(n + 1, -1);
        vector<char> ins(n + 1, false);
        vector<vector<int>> scc;
        int tim = 0;

        auto dfs = [&](auto&& go, int u) -> void {
            dfn[u] = low[u] = ++tim;
            stk.push_back(u);
            ins[u] = true;

            for (int v : g[u]) {
                if (!dfn[v]) {
                    go(go, v);
                    low[u] = min(low[u], low[v]);
                } else if (ins[v]) {
                    low[u] = min(low[u], dfn[v]);
                }
            }

            if (dfn[u] != low[u]) return;
            int id = scc.size();
            scc.push_back({});
            while (true) {
                int v = stk.back();
                stk.pop_back();
                ins[v] = false;
                bel[v] = id;
                scc.back().push_back(v);
                if (v == u) break;
            }
        };

        for (int u = 1; u <= n; ++u) {
            if (!dfn[u]) dfs(dfs, u);
        }

        vector<vector<int>> dag(scc.size());
        for (int u = 1; u <= n; ++u) {
            for (int v : g[u]) {
                int fr = bel[u], to = bel[v];
                if (fr != to) dag[fr].push_back(to);
            }
        }
        for (auto& e : dag) {
            sort(e.begin(), e.end());
            e.erase(unique(e.begin(), e.end()), e.end());
        }
        return {scc.size(), move(bel), move(scc), move(dag)};
    }
};
