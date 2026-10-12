for (int i = 1; i <= n; i++){
    for (int j = W; j >= 0; j--)
        for (int k = 0; k <= s[i]; k++){
            if (j - k * w[i] < 0) break;
            dp[j] = max(dp[j], dp[j - k * w[i]] + k * v[i]);
        }
