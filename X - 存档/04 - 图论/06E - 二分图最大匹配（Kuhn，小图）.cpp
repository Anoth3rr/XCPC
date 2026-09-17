/*
用途：DFS 增广路版二分图最大匹配，代码短，适合小图或左部较小的场景。
约定：左部为 1..nl，右部为 1..nr；add(l,r)。
接口：solve() 返回最大匹配；askCov(res) 返回一组最小点覆盖。
复杂度：O(nl * m)。大图优先 06A 的 Hopcroft-Karp。
*/

struct Kuhn {
    struct Result {
        int cnt;
        vector<int> ml, mr;
    };

    struct Cover {
        vector<char> l, r;
    };

    int nl, nr;
    vector<vector<int>> g;

    Kuhn(int nl, int nr) : nl(nl), nr(nr), g(nl + 1) {}

    void add(int l, int r) {
        assert(1 <= l && l <= nl && 1 <= r && r <= nr);
        g[l].push_back(r);
    }

    Result solve() const {
        vector<int> ml(nl + 1), mr(nr + 1), vis(nl + 1);
        int tim = 0, cnt = 0;
        auto dfs = [&](auto go, int u) -> bool {
            if (vis[u] == tim) return false;
            vis[u] = tim;
            for (int v : g[u]) {
                int x = mr[v];
                if (x && !go(go, x)) continue;
                ml[u] = v;
                mr[v] = u;
                return true;
            }
            return false;
        };
        for (int u = 1; u <= nl; ++u) {
            ++tim;
            if (dfs(dfs, u)) ++cnt;
        }
        return {cnt, move(ml), move(mr)};
    }

    Cover askCov(const Result& mat) const {
        vector<char> vl(nl + 1), vr(nr + 1);
        queue<int> q;
        for (int u = 1; u <= nl; ++u) {
            if (mat.ml[u]) continue;
            vl[u] = true;
            q.push(u);
        }
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : g[u]) {
                if (mat.ml[u] == v || vr[v]) continue;
                vr[v] = true;
                int x = mat.mr[v];
                if (x && !vl[x]) {
                    vl[x] = true;
                    q.push(x);
                }
            }
        }
        Cover res{vector<char>(nl + 1), vector<char>(nr + 1)};
        for (int u = 1; u <= nl; ++u) res.l[u] = !vl[u];
        for (int v = 1; v <= nr; ++v) res.r[v] = vr[v];
        return res;
    }
};
