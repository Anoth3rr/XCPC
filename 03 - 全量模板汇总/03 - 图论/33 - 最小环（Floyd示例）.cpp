int flody(int n) {
    for (int i = 1; i <= n; ++ i) {
        for (int j = 1; j <= n; ++ j) {
            val[i][j] = dis[i][j]; // 记录最初的边权值
        }
    }
    int ans = 0x3f3f3f3f;
    for (int k = 1; k <= n; ++ k) {
        for (int i = 1; i < k; ++ i) { // 注意这里是没有等于号的
            for (int j = 1; j < i; ++ j) {
                ans = min(ans, dis[i][j] + val[i][k] + val[k][j]);
            }
        }
    for (int i = 1; i <= n; ++ i) { // 往下是标准的flody
        for (int j = 1; j <= n; ++ j) {
                dis[i][j] = min(dis[i][j], dis[i][k] + dis[k][j]);
            }
        }
    }
    return ans;
}
