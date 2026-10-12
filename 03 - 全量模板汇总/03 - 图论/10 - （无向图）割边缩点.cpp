struct EDCC {
    int n, m, now, cnt;
    vector<vector<array<int, 2>>> ver;
    vector<int> dfn, low, col, S;
    set<array<int, 2>> bridge, direct; // 如果不需要，删除这一部分可以得到一些时间上的优化

    EDCC(int n) : n(n), low(n + 1), ver(n + 1), dfn(n + 1), col(n + 1) {
        m = now = cnt = 0;
    }
    void add(int x, int y) { // 和 scc 相比多了一条连边
        ver[x].push_back({y, m});
        ver[y].push_back({x, m++});
    }
    void tarjan(int x, int fa) {
        dfn[x] = low[x] = ++now;
        S.push_back(x);
        for (auto &[y, id] : ver[x]) {
            if (!dfn[y]) {
                direct.insert({x, y});
                tarjan(y, id);
                low[x] = min(low[x], low[y]);
                if (dfn[x] < low[y]) {
                    bridge.insert({x, y});
                }
            } else if (id != fa && dfn[y] < dfn[x]) {
                direct.insert({x, y});
                low[x] = min(low[x], dfn[y]);
            }
        }
        if (dfn[x] == low[x]) {
            int pre;
            cnt++;
            do {
                pre = S.back();
                col[pre] = cnt;
                S.pop_back();
            } while (pre != x);
        }
    }
    auto work() {
        for (int i = 1; i <= n; i++) { // 避免图不连通
            if (!dfn[i]) {
                tarjan(i, 0);
            }
        }
        /**
         * @param cnt 新图的顶点数量, adj 新图, col 旧图节点对应的新图节点
         * @param siz 旧图每一个边双中点的数量
         * @param bridge 全部割边, direct 非割边定向
         */
        vector<int> siz(cnt + 1);
        vector<vector<int>> adj(cnt + 1);
        for (int i = 1; i <= n; i++) {
            siz[col[i]]++;
            for (auto &[j, id] : ver[i]) {
                int x = col[i], y = col[j];
                if (x != y) {
                    adj[x].push_back(y);
                }
            }
        }
        return tuple{cnt, adj, col, siz};
    }
};
