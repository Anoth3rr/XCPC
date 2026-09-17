/*
用途：有向图从根 rt 出发的支配树；idom[v] 是 v 的直接支配点。
约定：点编号 1..n；不可达点和根的 idom 均为 0；允许重边、自环。
复杂度：实现使用 Lengauer-Tarjan，常用分析为 O((n+m) log n)，空间 O(n+m)。
注意：dfs 与 eval 为递归；链状图很深时需提高栈或改成迭代 DFS。
*/
struct DomTree {
    int n, ti;
    vector<vector<int>> g, rg, buc;
    vector<int> dfn, rev, par, sem, dom, anc, lab;

    explicit DomTree(int _n) : n(_n), g(_n + 1) {
        assert(n >= 0);
    }

    void add(int u, int v) {
        assert(1 <= u && u <= n && 1 <= v && v <= n);
        g[u].pb(v);
    }

    void dfs(int u) {
        dfn[u] = ++ti;
        rev[ti] = u;
        sem[ti] = lab[ti] = ti;
        for (int v : g[u]) {
            if (!dfn[v]) {
                dfs(v);
                par[dfn[v]] = dfn[u];
            }
            rg[dfn[v]].pb(dfn[u]);
        }
    }

    int find(int x) {
        if (anc[x] == x) return x;
        int y = anc[x];
        int z = find(y);
        if (sem[lab[y]] < sem[lab[x]]) lab[x] = lab[y];
        return anc[x] = z;
    }

    int eval(int x) {
        find(x);
        return lab[x];
    }

    vector<int> ask(int rt) {
        assert(1 <= rt && rt <= n);
        ti = 0;
        dfn.assign(n + 1, 0);
        rev.assign(n + 1, 0);
        par.assign(n + 1, 0);
        sem.assign(n + 1, 0);
        dom.assign(n + 1, 0);
        anc.assign(n + 1, 0);
        lab.assign(n + 1, 0);
        rg.assign(n + 1, {});
        buc.assign(n + 1, {});
        dfs(rt);
        for (int i = 1; i <= ti; ++i) anc[i] = lab[i] = i;
        for (int i = ti; i >= 2; --i) {
            for (int v : rg[i]) sem[i] = min(sem[i], sem[eval(v)]);
            buc[sem[i]].pb(i);
            for (int v : buc[par[i]]) {
                int u = eval(v);
                dom[v] = sem[u] < sem[v] ? u : par[i];
            }
            buc[par[i]].clear();
            anc[i] = par[i];
        }
        for (int i = 2; i <= ti; ++i)
            if (dom[i] != sem[i]) dom[i] = dom[dom[i]];
        vector<int> r(n + 1);
        for (int i = 2; i <= ti; ++i) r[rev[i]] = rev[dom[i]];
        return r;
    }
};
