struct HeavyLD {
    ll n;
    ll dtot;
    vector<ll> w; // 编号索引下的点权
    vector<ll> sz, deps, fa, mxson; // mxson: 重儿子
    vector<ll> dfn, enddfn, tops, weis; // tops: 重链的链顶, weis: dfn索引下的点权
    SegTree seg;
    vector<vector<ll>> g;
    HeavyLD(ll n_) : n(n_), seg(n_) { // seg本身就是 1-indexed, 所以大小为 n 就行
        w.resize(n+1); g.resize(n+1);
        sz.resize(n+1); deps.resize(n+1); fa.resize(n+1); mxson.resize(n+1); 
        dfn.resize(n+1); enddfn.resize(n+1); tops.resize(n+1); weis.resize(n+1);
    }

    void initw(vector<ll> &a) { // 初始化点权权重
        for (int i = 1; i <= n; i++) {
            w[i] = a[i];
        }
    }

    void adde(ll u, ll v) { // 给图加边 (邻接表)
        g[u].pb(v); g[v].pb(u);
    }

    void pdfs(ll u, ll f) { // pdfs: p(pre) 预处理dfs
        deps[u] = deps[f] + 1;
        fa[u] = f;
        sz[u] = 1;
        for (auto v: g[u]) {
            if (v == f) continue;
            pdfs(v, u);
            sz[u] += sz[v];
            if (sz[v] > sz[mxson[u]]) mxson[u] = v;
        }
    };

    
    
    void pdfs2(ll u, ll top) {
        tops[u] = top; dfn[u] = ++dtot; weis[dtot] = w[u];
        if (mxson[u]) pdfs2(mxson[u],top);
        for (auto v: g[u]) {
            if (v == fa[u] || v == mxson[u]) continue;
            pdfs2(v, v);
        }
        enddfn[u] = dtot;
    };

    void init(){ // 干完所有活之后, 再进行初始化树链剖分 (也不要最后忘记跑这个导致树链剖分一点都没跑)
        dtot = 0;
        pdfs(1, 0);
        pdfs2(1, 1);
        seg.init(weis);
    }
    
    void updpath(ll u, ll v, ll k) { // u~v 的路径每个点 + k
        while (tops[u] != tops[v]) {
            if (deps[tops[u]] < deps[tops[v]]) swap(u, v);
            seg.upd(dfn[tops[u]],dfn[u], k);
            u = tops[u];
            u = fa[u];
        }
        if (deps[u] < deps[v]) swap(u, v);
        seg.upd(dfn[v],dfn[u], k);
    };

    void updsubtree(ll u, ll k) { // u~v 的子树每个点 + k
        seg.upd(dfn[u], enddfn[u], k);
    };

    void updpoint(ll u, ll k) { // 更改 u 的点权
        seg.upd(dfn[u], dfn[u], k);
    };

    ll askpath(ll u, ll v) { // 求 u~v 的路径每个点点权和
        ll res = 0;
        while (tops[u] != tops[v]) {
            if (deps[tops[u]] < deps[tops[v]]) swap(u, v);
            res += seg.ask(dfn[tops[u]],dfn[u]).val;
            u = tops[u];
            u = fa[u];
        }
        if (deps[u] < deps[v]) swap(u, v);
        res += seg.ask(dfn[v],dfn[u]).val;
        return res;
    };

    ll asksubtree(ll u) { // 询问 u 的子树每个点的点权和
        ll res = seg.ask(dfn[u], enddfn[u]).val;
        return res;
    };

    ll askpoint(ll u) { // 询问 u 的点权
        ll res = seg.ask(dfn[u], dfn[u]).val;
        return res;
    };
};