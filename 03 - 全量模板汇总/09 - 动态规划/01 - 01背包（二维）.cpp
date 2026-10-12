for (int i = 1; i <= n; i++)
    for (int j = 0; j <= W; j++){
        dp[i][j] = dp[i - 1][j];
        if (j >= w[i])
            dp[i][j] = max(dp[i][j], dp[i - 1][j - w[i]] + v[i]);
    }
