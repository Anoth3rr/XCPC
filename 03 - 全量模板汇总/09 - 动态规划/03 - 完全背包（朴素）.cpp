for (int i = 1; i <= n; i++)
    for (int j = 0; j <= W; j++)
        for (int k = 0; k * w[i] <= j; k++)    //选取几个物品 
            dp[i][j] = max(dp[i][j], dp[i - 1][j - k * w[i]] + k * v[i]);
