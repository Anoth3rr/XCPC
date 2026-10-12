// 结构体中增加
int check(int x, int y) {
    vector<int> vis(2 * n);
    auto dfs = [&](auto self, int x) -> void {
        vis[x] = 1;
        for (auto y : e[x]) {
            if (vis[y]) continue;
            self(self, y);
        }
    };
    dfs(dfs, x);
    return vis[y];
}
// 主函数中增加
for (int i = 0; i < n; i++) {
    if (sat.check(2 * i, 2 * i + 1)) {
        cout << 1 << " ";
    } else if (sat.check(2 * i + 1, 2 * i)) {
        cout << 0 << " ";
    } else {
        cout << "?" << " ";
    }
}
