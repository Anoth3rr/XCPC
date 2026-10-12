for (int i = 1; i <= n; i++)  //当前装第 i 件物品
    for (int j = W; j >= w[i]; j--)  //背包容量为 j
        dp[j] = max(dp[j], dp[j - w[i]] + v[i]);  //判断背包容量为 j 的情况下能是实现总价值最大是多少
