signed main() {
    int Task = 1;
    for (cin >> Task; Task; Task--) {
        int n, m;
        cin >> n >> m;
 
        vector<int> w(n + 1), v(n + 1);
        vector<vector<int>> dp(n + 2, vector<int>(m + 2));
        for (int i = 1; i <= n; i++) {
            cin >> w[i] >> v[i];
        }
 
        for (int i = n; i >= 1; i--) {
            for (int j = 0; j <= m; j++) {
                dp[i][j] = dp[i + 1][j];
                if (j >= w[i]) {
                    dp[i][j] = max(dp[i][j], dp[i + 1][j - w[i]] + v[i]);
                }
            }
        }
 
        vector<int> ans;
        for (int i = 1; i <= n; i++) {
            if (m - w[i] >= 0 && dp[i][m] == dp[i + 1][m - w[i]] + v[i]) {
                ans.push_back(i);
                // cout << i << " ";
                m -= w[i];
            }
        }
        cout << ans.size() << "\n";
        for (auto i : ans) {
            cout << i << " ";
        }
        cout << "\n";
    }
}
