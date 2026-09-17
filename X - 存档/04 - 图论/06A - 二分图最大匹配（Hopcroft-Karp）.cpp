/*
用途：二分图最大匹配，并可由最大匹配求最小点覆盖（Kőnig 定理）。
约定：左部编号 1..nl，右部编号 1..nr；add(l,r)。
复杂度：最大匹配 O(m sqrt(n))；askCov 为 O(n + m)。
*/
struct HK {
    struct Result {
        int cnt;
        vector<int> ml;   // matchLeft[u] = 匹配到的右部点，0 表示未匹配。
        vector<int> mr;  // matchRight[v] = 匹配到的左部点，0 表示未匹配。
    };

    struct Cover {
        vector<char> l;
        vector<char> r;
    };

    int nl, nr;
    vector<vector<int>> g;

    HK(int nl, int nr) : nl(nl), nr(nr), g(nl + 1) {}

    void add(int l, int r) {
        assert(1 <= l && l <= nl && 1 <= r && r <= nr);
        g[l].push_back(r);
    }

    Result solve() const {
        const int INF = numeric_limits<int>::max();
        vector<int> ml(nl + 1), mr(nr + 1), dis(nl + 1);

        auto bfs = [&]() -> bool {
            queue<int> q;
            for (int u = 1; u <= nl; ++u) {
                if (ml[u] == 0) {
                    dis[u] = 0;
                    q.push(u);
                } else {
                    dis[u] = INF;
                }
            }

            int lim = INF;
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                if (dis[u] >= lim) continue;
                for (int v : g[u]) {
                    int x = mr[v];
                    if (x == 0) {
                        lim = dis[u] + 1;
                    } else if (dis[x] == INF) {
                        dis[x] = dis[u] + 1;
                        q.push(x);
                    }
                }
            }
            return lim != INF;
        };

        auto dfs = [&](auto&& go, int u) -> bool {
            for (int v : g[u]) {
                int x = mr[v];
                if (x == 0 || (dis[x] == dis[u] + 1 && go(go, x))) {
                    ml[u] = v;
                    mr[v] = u;
                    return true;
                }
            }
            dis[u] = INF;
            return false;
        };

        int cnt = 0;
        while (bfs()) {
            for (int u = 1; u <= nl; ++u) {
                if (ml[u] == 0 && dfs(dfs, u)) ++cnt;
            }
        }
        return {cnt, move(ml), move(mr)};
    }

    // 传入 solve 的结果；返回（左部未访问点）+（右部访问点）。
    Cover askCov(const Result& mat) const {
        vector<char> vl(nl + 1, false), vr(nr + 1, false);
        queue<int> q;
        for (int u = 1; u <= nl; ++u) {
            if (mat.ml[u] == 0) {
                vl[u] = true;
                q.push(u);
            }
        }

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : g[u]) {
                if (mat.ml[u] == v || vr[v]) continue;
                vr[v] = true;
                int x = mat.mr[v];
                if (x != 0 && !vl[x]) {
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
