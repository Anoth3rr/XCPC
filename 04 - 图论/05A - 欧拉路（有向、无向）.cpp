/*
用途：Hierholzer 求欧拉通路/回路，自动检查度数条件与非孤立部分连通性。
约定：点编号 1..n；构造时指定 dir。返回的点列长度为 m + 1；无解返回 nullopt。
没有边时返回 {1}。支持重边和自环。
复杂度：O(n + m)。
*/
struct EulerTrail {
    int n;
    bool dir;
    int m = 0;
    vector<vector<pair<int, int>>> g;
    vector<int> in, out, deg;

    EulerTrail(int n, bool dir)
        : n(n), dir(dir), g(n + 1), in(n + 1), out(n + 1), deg(n + 1) {}

    int add(int u, int v) {
        int id = m++;
        g[u].push_back({v, id});
        if (dir) {
            ++out[u];
            ++in[v];
        } else {
            g[v].push_back({u, id});
            ++deg[u];
            ++deg[v];
        }
        return id;
    }

    optional<vector<int>> solve() const {
        if (m == 0) return vector<int>{1};
        int s = -1;

        if (dir) {
            int cs = 0, ct = 0;
            for (int u = 1; u <= n; ++u) {
                int d = out[u] - in[u];
                if (d == 1) {
                    ++cs;
                    s = u;
                } else if (d == -1) {
                    ++ct;
                } else if (d != 0) {
                    return nullopt;
                }
            }
            if (cs != ct || cs > 1) return nullopt;
            if (s == -1) {
                for (int u = 1; u <= n; ++u) {
                    if (out[u] > 0) {
                        s = u;
                        break;
                    }
                }
            }
        } else {
            int odd = 0;
            for (int u = 1; u <= n; ++u) {
                if (deg[u] & 1) {
                    ++odd;
                    s = u;
                }
            }
            if (odd != 0 && odd != 2) return nullopt;
            if (s == -1) {
                for (int u = 1; u <= n; ++u) {
                    if (deg[u] > 0) {
                        s = u;
                        break;
                    }
                }
            }
        }

        vector<int> it(n + 1);
        vector<char> vis(m, false);
        vector<int> stk = {s}, pth;
        while (!stk.empty()) {
            int u = stk.back();
            while (it[u] < g[u].size() && vis[g[u][it[u]].second]) {
                ++it[u];
            }
            if (it[u] == g[u].size()) {
                pth.push_back(u);
                stk.pop_back();
            } else {
                auto [v, id] = g[u][it[u]++];
                vis[id] = true;
                stk.push_back(v);
            }
        }
        if (pth.size() != m + 1) return nullopt;
        reverse(pth.begin(), pth.end());
        return pth;
    }
};
