int n, m;
cin >> n >> m;
vector<vector<int>> G(n);
for (int i = 0; i < m; i++) {
    int u, v;
    cin >> u >> v;
    u--, v--;
    G[u].push_back(v);
    G[v].push_back(u);
}
vector<vector<LL>> dp(1 << n, vector<LL>(n));
for (int i = 0; i < n; i++) dp[1 << i][i] = 1;
LL ans = 0;
for (int st = 1; st < (1 << n); st++) {
    for (int u = 0; u < n; u++) {
        if (!dp[st][u]) continue;
        int start = st & -st;
        for (auto v : G[u]) {
            if ((1 << v) < start) continue;
            if ((1 << v) & st) {
                if ((1 << v) == start) {
                    ans += dp[st][u];
                }
            } else {
                dp[st | (1 << v)][v] += dp[st][u];
            }
        }
    }
}
cout << (ans - m) / 2 << "\n";
