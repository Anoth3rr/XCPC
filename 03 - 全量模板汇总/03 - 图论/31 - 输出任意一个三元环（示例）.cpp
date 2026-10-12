int n;
cin >> n;
vector<vector<int>> a(n + 1, vector<int>(n + 1));
for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= n; ++j) {
        char x;
        cin >> x;
        if (x == '1') a[i][j] = 1;
    }
}

vector<int> vis(n + 1);
function<void(int, int)> dfs = [&](int x, int fa) {
    vis[x] = 1;
    for (int y = 1; y <= n; ++y) {
        if (a[x][y] == 0) continue;
        if (a[y][fa] == 1) {
            cout << fa << " " << x << " " << y;
            exit(0);
        }
        if (!vis[y]) dfs(y, x); // 这一步的if判断很关键
    }
};
for (int i = 1; i <= n; ++i) {
    if (!vis[i]) dfs(i, -1);
}
cout << -1;
