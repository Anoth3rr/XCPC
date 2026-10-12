LL Min = 1e18, ans = 0;
for (int k = 1; k <= n; k++) {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (dis[i][j] > dis[i][k] + dis[k][j]) {
                dis[i][j] = dis[i][k] + dis[k][j];
                cnt[i][j] = cnt[i][k] * cnt[k][j] % mod;
            } else if (dis[i][j] == dis[i][k] + dis[k][j]) {
                cnt[i][j] = (cnt[i][j] + cnt[i][k] * cnt[k][j] % mod) % mod;
            }
        }
    }
    for (int i = 1; i < k; i++) {
        if (a[k][i]) {
            if (a[k][i] + dis[i][k] < Min) {
                Min = a[k][i] + dis[i][k];
                ans = cnt[i][k];
            } else if (a[k][i] + dis[i][k] == Min) {
                ans = (ans + cnt[i][k]) % mod;
            }
        }
    }
}
