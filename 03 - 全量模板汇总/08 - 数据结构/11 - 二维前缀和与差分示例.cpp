vector<vector<i64>> d(n + 2, vector<i64>(m + 2));
for (int x = 1; x <= n; x++) {
    for (int y = 1; y <= m; y++) { // 由初始矩阵 a 构造二维差分
        d[x][y] = a[x][y] - a[x - 1][y] - a[x][y - 1] + a[x - 1][y - 1];
    }
}
auto matrixAdd = [&](int u, int v, int x, int y, i64 p) { // [u, v] ~ [x, y] 全部加 p
    x++, y++;
    d[u][v] += p, d[x][v] -= p;
    d[x][y] += p, d[u][y] -= p;
};

// 所有修改完成后还原 a
for (int x = 1; x <= n; x++) {
    for (int y = 1; y <= m; y++) {
        d[x][y] += d[x - 1][y] + d[x][y - 1] - d[x - 1][y - 1];
        a[x][y] = d[x][y];
    }
}

vector<vector<i64>> pre(n + 1, vector<i64>(m + 1));
for (int x = 1; x <= n; x++) {
    for (int y = 1; y <= m; y++) {
        pre[x][y] = a[x][y] + pre[x - 1][y] + pre[x][y - 1] - pre[x - 1][y - 1];
    }
}
auto matrixSum = [&](int u, int v, int x, int y) {
    return pre[x][y] - pre[u - 1][y] - pre[x][v - 1] + pre[u - 1][v - 1];
};
