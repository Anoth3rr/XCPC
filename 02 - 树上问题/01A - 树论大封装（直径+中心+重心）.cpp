/*
用途：带非负边权树的基础树论封装。

支持：
- 直径端点与直径长度；
- 顶点树心（使到最远顶点距离最小的点）与半径；
- 树重心（删去该点后最大连通块最小）；
- 指定根下的 parent / dep / maxdep。

约定：
- 点编号为 1..n，树连通；边权必须非负。
- center 是“顶点树心”。若题目允许把中心放在边的内部，需要在直径上另行二分位置。

接口：
- add(u, v, w = 1)：加入无向边。
- work(root = 1)：一次性完成全部预处理。
- askEnds()：返回一组直径端点，同时更新 diam。
*/

struct Tree {

    int n;
    vector<vector<pair<int, int>>> e;

    // dep 为无权深度；maxdep 为该根树内最大的无权深度。
    vector<int> dep, fa, mxd;
    // d1 / d2：向下第一、第二长距离；up：经过父边到树外的最长距离。
    vector<int> d1, d2, up;
    vector<int> s1, s2;
    vector<bool> vis;
    int rem, cog;
    int rad, dia;
    int cen;

    explicit Tree(int n) : n(n), e(n + 1), dep(n + 1), fa(n + 1),
                           mxd(n + 1), d1(n + 1), d2(n + 1), up(n + 1),
                           s1(n + 1), s2(n + 1), vis(n + 1) {}

    void add(int u, int v, int w = 1) {
        e[u].push_back({v, w});
        e[v].push_back({u, w});
    }

    void dfs(int u, int _fa) {
        fa[u] = _fa;
        mxd[u] = dep[u];
        for (auto [v, w] : e[u]) {
            if (v == _fa) continue;
            dep[v] = dep[u] + 1;
            dfs(v, u);
            mxd[u] = max(mxd[u], mxd[v]);
        }
    }

    void dfs1(int u, int fa) {
        for (auto [v, w] : e[u]) {
            if (v == fa) continue;
            dfs1(v, u);
            int cur = d1[v] + w;
            if (cur > d1[u]) {
                d2[u] = d1[u], s2[u] = s1[u];
                d1[u] = cur, s1[u] = v;
            } else if (cur > d2[u]) {
                d2[u] = cur, s2[u] = v;
            }
        }
    }

    void dfs2(int u, int fa) {
        for (auto [v, w] : e[u]) {
            if (v == fa) continue;
            up[v] = (s1[u] == v ? max(up[u], d2[u]) : max(up[u], d1[u])) + w;
            dfs2(v, u);
        }
    }

    void calCen() {
        cen = 1;
        for (int u = 1; u <= n; ++u) {
            if (max(d1[u], up[u]) < max(d1[cen], up[cen])) cen = u;
        }
        rad = max(d1[cen], up[cen]);

        dia = 0;
        for (int u = 1; u <= n; ++u) {
            dia = max(dia, d1[u] + max(d2[u], up[u]));
        }
    }

    int dfsCog(int u, int fa) {
        int siz = 1, mx = 0;
        for (auto [v, w] : e[u]) {
            if (v == fa) continue;
            int sub = dfsCog(v, u);
            siz += sub;
            mx = max(mx, sub);
        }
        mx = max(mx, n - siz);
        if (mx < rem) rem = mx, cog = u;
        return siz;
    }

    void calCog() {
        rem = numeric_limits<int>::max();
        cog = 1;
        dfsCog(1, 0);
    }

    pair<int, vector<int>> far(int src) const {
        vector<int> fa(n + 1, -1), st = {src};
        vector<int> dis(n + 1);
        fa[src] = 0;
        for (int i = 0; i < st.size(); ++i) {
            int u = st[i];
            for (auto [v, w] : e[u]) {
                if (v == fa[u]) continue;
                fa[v] = u;
                dis[v] = dis[u] + w;
                st.push_back(v);
            }
        }

        int far = src;
        for (int u = 1; u <= n; ++u) {
            if (dis[u] > dis[far]) far = u;
        }
        return {far, dis};
    }

    pair<int, int> askEnds() {
        auto [u, ban] = far(1);
        auto [v, dis] = far(u);
        dia = dis[v];
        return {u, v};
    }

    void work(int rt = 1) {
        dep.assign(n + 1, 0);
        fa.assign(n + 1, 0);
        mxd.assign(n + 1, 0);
        d1.assign(n + 1, 0);
        d2.assign(n + 1, 0);
        up.assign(n + 1, 0);
        s1.assign(n + 1, 0);
        s2.assign(n + 1, 0);

        dfs(rt, 0);
        dfs1(rt, 0);
        dfs2(rt, 0);
        calCen();
        calCog();
        askEnds();
    }
};
