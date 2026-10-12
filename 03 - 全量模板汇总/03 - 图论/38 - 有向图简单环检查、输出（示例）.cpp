signed main() {
    int n, m;
    cin >> n >> m;
 
    vector<vector<array<int, 2>>> ver(n + 1);
    for (int i = 1; i <= m; i++) {
        int x, y;
        cin >> x >> y;
        ver[x].push_back({y, i});
    }
     
    vector<int> vis(n + 1);
    vector<array<int, 2>> fa(n + 1);
    auto dfs = [&](auto self, int x, int from) -> void {
        vis[x] = 1;
        for (auto [y, id] : ver[x]) {
            if (id == from) continue;
            if (!vis[y]) {
                fa[y] = {x, id};
                self(self, y, id);
            } else if (vis[y] == 1) {
                vector<int> V = {y}, E = {id};
                for (int pre = x; pre != y; pre = fa[pre][0]) {
                    V.push_back(pre);
                    E.push_back(fa[pre][1]);
                }
 
                int l = V.size();
                cout << l << "\n";
                reverse(V.begin(), V.end());
                reverse(E.begin(), E.end());
                rotate(E.begin(), E.begin() + 1, E.end());
                for (int i = 0; i < l; i++) {
                    cout << V[i] << " \n"[i == l - 1];
                }
                for (int i = 0; i < l; i++) {
                    cout << E[i] << " \n"[i == l - 1];
                }
                exit(0);
            }
        }
        vis[x] = 2;
    };
    for (int i = 1; i <= n; i++) {
        if (!vis[i]) {
            dfs(dfs, i, -1);
        }
    }
 
    cout << -1 << "\n";
}
