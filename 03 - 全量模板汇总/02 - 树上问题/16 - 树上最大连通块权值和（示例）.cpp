LL n, point[N];
LL ver[N], head[N], nex[N], tot; bool v[N];
map<pair<LL, LL>, LL> edge;
// void add(LL x, LL y) {}
void dfs(LL x) {
    for (LL i = head[x]; i; i = nex[i]) {
        LL y = ver[i];
        if (v[y]) continue;
        v[y] = true; dfs(y); v[y] = false;
    }
    for (LL i = head[x]; i; i = nex[i]) {
        LL y = ver[i];
        if (v[y]) continue;
        point[x] += max(point[y] + edge[{x, y}], 0LL);
    }
}
void Solve() {
    cin >> n;
    FOR(i, 1, n) cin >> point[i];
    FOR(i, 2, n) {
        LL x, y, w; cin >> x >> y >> w;
        edge[{x, y}] = edge[{y, x}] = w;
        add(x, y), add(y, x);
    }
    v[1] = true; dfs(1); LL ans = -MAX18;
    FOR(i, 1, n) ans = max(ans, point[i]);
    cout << ans << endl;
}
