for (int i = 1; i <= n; i++)
    for (int j = W; j >= w; j--)  //容量限制
        for (int k = M; k >= m; k--)  //重量限制
            dp[j][k] = max(dp[j][k], dp[j - w][k - m] + v);
