signed main() {
    int n, m;
    cin >> n >> m;
    
    DSU dsu(n); // 这里引入DSU判断图是否联通，如题目有保证，则不需要此步骤
    vector<vector<int>> edge(n + 1, vector<int>(n + 1));
    for (int i = 1; i <= m; i++) {
        int x, y, w;
        cin >> x >> y >> w;
        dsu.merge(x, y);
        edge[x][y] += w;
        edge[y][x] += w;
    }
    
    if (dsu.Poi(1) != n || m < n - 1) { // 图不联通
        cout << 0 << endl;
        return 0;
    }
    
    int MinCut = INF, S = 1, T = 1; // 虚拟源汇点
    vector<int> bin(n + 1);
    auto contract = [&]() -> int { // 求解S到T的最小割，定义为 cut of phase
        vector<int> dis(n + 1), vis(n + 1);
        int Min = 0;
        for (int i = 1; i <= n; i++) {
            int k = -1, maxc = -1;
            for (int j = 1; j <= n; j++) {
                if (!bin[j] && !vis[j] && dis[j] > maxc) {
                    k = j;
                    maxc = dis[j];
                }
            }
            if (k == -1) return Min;
            S = T, T = k, Min = maxc;
            vis[k] = 1;
            for (int j = 1; j <= n; j++) {
                if (!bin[j] && !vis[j]) {
                    dis[j] += edge[k][j];
                }
            }
        }
        return Min;
    };
    for (int i = 1; i < n; i++) { // 这里取不到等号
        int val = contract();
        bin[T] = 1;
        MinCut = min(MinCut, val);
        if (!MinCut) {
            cout << 0 << endl;
            return 0;
        }
        for (int j = 1; j <= n; j++) {
            if (!bin[j]) {
                edge[S][j] += edge[j][T];
                edge[j][S] += edge[j][T];
            }
        }
    }
    cout << MinCut << endl;
}
