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
