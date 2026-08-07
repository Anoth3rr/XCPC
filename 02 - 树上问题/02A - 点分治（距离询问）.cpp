/*
用途：点分治模板，常用于统计/判断树上点对距离，当前写法用于回答是否存在距离为 q[i] 的点对。
依赖：
- 外部已有 n, m。
- ver[u] 存边 {v, w}。
- q[1..m] 存询问距离，ans[1..m] 会被置为 1 表示存在。
接口/流程：
- dfsSz(x, fa)：计算当前未删除连通块大小。
- cen(x, fa, total)：在大小为 total 的连通块内找重心 root。
- calc(x)：处理所有经过重心 x 的路径贡献。
- dfz(root, 0)：从重心 root 开始分治。
*/
int rt = 0, mt = 0;
vector<int> vis(n + 1), siz(n + 1);

auto fsz = [&](auto go, int x, int fa) -> int {
    siz[x] = 1;
    for (auto [y, w] : ver[x]) {
        if (y == fa || vis[y]) continue;
        siz[x] += go(go, y, x);
    }
    return siz[x];
};

auto cen = [&](auto go, int x, int fa, int tot) -> void {
    siz[x] = 1;
    int val = 0;
    for (auto [y, w] : ver[x]) {
        if (y == fa || vis[y]) continue;
        go(go, y, x, tot);
        siz[x] += siz[y];
        val = max(val, siz[y]);
    }
    val = max(val, tot - siz[x]);
    if (val < mt) {
        mt = val;
        rt = x;
    }
};

auto cal = [&](int x) -> void {
    set<int> pre = {0};
    vector<int> dis(n + 1);
    for (auto [y, w] : ver[x]) {
        if (vis[y]) continue;
        vector<int> son;
        auto dfs = [&](auto go, int u, int fa) -> void {
            son.push_back(dis[u]);
            for (auto [y, w] : ver[u]) {
                if (y == fa || vis[y]) continue;
                dis[y] = dis[u] + w;
                go(go, y, u);
            }
        };
        dis[y] = w;
        dfs(dfs, y, x);

        for (auto it : son) {
            for (int i = 1; i <= m; i++) {
                if (q[i] < it || !pre.count(q[i] - it)) continue;
                ans[i] = 1;
            }
        }
        pre.insert(son.begin(), son.end());
    }
};

auto dfz = [&](auto go, int x, int fa) -> void {
    vis[x] = 1;
    cal(x);
    for (auto [y, w] : ver[x]) {
        if (y == fa || vis[y]) continue;
        int tot = fsz(fsz, y, x);
        rt = 0;
        mt = tot;
        cen(cen, y, x, tot);
        go(go, rt, x);
    }
};

int tot = fsz(fsz, 1, 0);
rt = 0;
mt = tot;
cen(cen, 1, 0, tot);
dfz(dfz, rt, 0);
