/*
用途：布尔可满足性（每个子句至多两个文字）。
约定：变量编号 0..m-1；lit(x,true) 表示 x，lit(x,false) 表示 !x。
接口：add2(x,vx,y,vy) 添加 (x == vx) OR (y == vy)，force(x,vx) 强制取值。
复杂度：O(V + E)。solve() 返回 nullopt 表示无解，否则 ans[x] 是变量 x 的取值。
*/
struct TwoSAT {
    int m;
    vector<vector<int>> g;

    explicit TwoSAT(int m) : m(m), g(2 * m) {}

    int lit(int x, bool val) const {
        assert(0 <= x && x < m);
        return 2 * x + (val ? 0 : 1);
    }

    static int neg(int id) {
        return id ^ 1;
    }

    void add(int fr, int to) {
        g[fr].push_back(to);
    }

    void addOr(int a, int b) {
        add(neg(a), b);
        add(neg(b), a);
    }

    void add2(int x, bool vx, int y, bool vy) {
        addOr(lit(x, vx), lit(y, vy));
    }

    void force(int x, bool val) {
        int a = lit(x, val);
        addOr(a, a);
    }

    optional<vector<char>> solve() const {
        const int n = 2 * m;
        vector<int> dfn(n), low(n), stk, bel(n, -1);
        vector<char> ins(n, false);
        int tim = 0, cc = 0;

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
            while (true) {
                int v = stk.back();
                stk.pop_back();
                ins[v] = false;
                bel[v] = cc;
                if (v == u) break;
            }
            ++cc;
        };

        for (int u = 0; u < n; ++u) {
            if (!dfn[u]) dfs(dfs, u);
        }

        vector<char> ans(m);
        for (int x = 0; x < m; ++x) {
            int vx = lit(x, true);
            int vy = lit(x, false);
            if (bel[vx] == bel[vy]) return nullopt;
            // Tarjan 的分量按出边方向从后向前编号，编号更小者应优先取真。
            ans[x] = bel[vx] < bel[vy];
        }
        return ans;
    }
};
