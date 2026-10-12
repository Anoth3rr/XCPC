int n, m, s;
cin >> n >> m >> s;

vector<tuple<int, int, i64>> ver(m + 1);
for (int i = 1; i <= m; ++i) {
    int x, y;
    i64 w;
    cin >> x >> y >> w;
    ver[i] = {x, y, w};
}

vector<i64> dis(n + 1, inf), chk(n + 1);
dis[s] = 0;
for (int i = 1; i <= 2 * n; ++i) { // 双倍松弛，获取负环信息
    vector<i64> backup = dis;
    for (int j = 1; j <= m; ++j) {
        auto [x, y, w] = ver[j];
        chk[y] |= (i > n && backup[x] + w < dis[y]);
        dis[y] = min(dis[y], backup[x] + w);
    }
}
 
for (int i = 1; i <= n; ++i) {
    if (i == s) {
        cout << 0 << " ";
    } else if (dis[i] >= inf / 2) {
        cout << "no ";
    } else if (chk[i]) {
        cout << "inf ";
    } else {
        cout << dis[i] << " ";
    }
}
