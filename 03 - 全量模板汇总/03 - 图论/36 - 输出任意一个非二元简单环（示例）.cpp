vector<int> dis(n + 1, -1), fa(n + 1);
auto dfs = [&](auto self, int x) -> void {
    for (auto y : ver[x]) {
        if (y == fa[x]) continue; // 二元环需删去该行
        if (dis[y] == -1) {
            dis[y] = dis[x] + 1;
            fa[y] = x;
            self(self, y);
        } else if (dis[y] < dis[x]) {
            cout << dis[x] - dis[y] + 1 << endl;
            int pre = x;
            cout << pre << " ";
            while (pre != y) {
                pre = fa[pre];
                cout << pre << " ";
            }
            cout << endl;
            exit(0);
        }
    }
};
for (int i = 1; i <= n; i++) {
    if (dis[i] == -1) {
        dis[i] = 0;
        dfs(dfs, 1);
    }
}
