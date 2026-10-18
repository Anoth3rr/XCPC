vector<PLL> build(vector<ll> points) {
    vector<PLL> g_res;
    sort(all(points), [&](ll &a, ll &b) {
        return dfn[a] < dfn[b];
    });
    vector<ll> stk;
    stk.pb(1);
    if (points[0] != 1) stk.pb(points[0]);
    for (int i = 1; i < points.size(); i++) {
        ll u = points[i];
        ll lca = getlca(stk.back(), u);
        while (stk.size() > 1 && deps[stk[stk.size()-2]] >= deps[lca]) {
            ll v1 = stk.back(), v2 = stk[stk.size()-2];
            g_res.pb({v1, v2});
            stk.pop_back();
        }
        if (lca != stk.back()) {
            ll v1 = stk.back(), v2 = lca;
            g_res.pb({v1, v2});
            stk.pop_back();  
            stk.pb(lca);
        }
        stk.pb(u);
    }
    while (stk.size() > 1) {
        ll v1 = stk[stk.size()-2], v2 = stk.back();
        g_res.pb({v1, v2});
        stk.pop_back();
    }
    return g_res;
}