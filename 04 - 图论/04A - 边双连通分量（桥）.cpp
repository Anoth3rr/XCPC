/*
用途：无向图找桥、删桥后的边双连通分量，以及桥树（非连通图时为桥森林）。
约定：点编号 1..n；支持重边，边编号为 add 的返回值（0..m-1）。
复杂度：O(n + m)。
*/
struct EBCC {
    struct Result {
        int cc;
        vector<int> bel;          // 点所属边双连通分量，编号 0..componentCount-1。
        vector<char> br;          // 对应 add 的边编号。
        vector<vector<int>> tr; // 分量编号上的桥森林。
    };

    int n;
    vector<pair<int, int>> e;
    vector<vector<pair<int, int>>> g;

    explicit EBCC(int n) : n(n), g(n + 1) {}

    int add(int u, int v) {
        int id = e.size();
        e.push_back({u, v});
        g[u].push_back({v, id});
        g[v].push_back({u, id});
        return id;
    }

    Result solve() const {
        const int m = e.size();
        vector<int> dfn(n + 1), low(n + 1);
        vector<char> br(m, false);
        int tim = 0;

        auto dfs = [&](auto&& go, int u, int pe) -> void {
            dfn[u] = low[u] = ++tim;
            for (auto [v, id] : g[u]) {
                if (id == pe) continue;
                if (!dfn[v]) {
                    go(go, v, id);
                    low[u] = min(low[u], low[v]);
                    if (low[v] > dfn[u]) br[id] = true;
                } else {
                    low[u] = min(low[u], dfn[v]);
                }
            }
        };

        for (int u = 1; u <= n; ++u) {
            if (!dfn[u]) dfs(dfs, u, -1);
        }

        vector<int> bel(n + 1, -1);
        int cc = 0;
        for (int s = 1; s <= n; ++s) {
            if (bel[s] != -1) continue;
            stack<int> st;
            st.push(s);
            bel[s] = cc;
            while (!st.empty()) {
                int u = st.top();
                st.pop();
                for (auto [v, id] : g[u]) {
                    if (br[id] || bel[v] != -1) continue;
                    bel[v] = cc;
                    st.push(v);
                }
            }
            ++cc;
        }

        vector<vector<int>> tr(cc);
        for (int id = 0; id < m; ++id) {
            if (!br[id]) continue;
            auto [u, v] = e[id];
            int x = bel[u], y = bel[v];
            tr[x].push_back(y);
            tr[y].push_back(x);
        }
        return {cc, move(bel), move(br), move(tr)};
    }
};
