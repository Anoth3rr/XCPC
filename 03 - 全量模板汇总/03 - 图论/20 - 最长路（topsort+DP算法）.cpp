struct DAG {
    int n;
    vector<vector<pair<int, int>>> ver;
    vector<int> deg, dis;
    DAG(int n) : n(n) {
        ver.resize(n + 1);
        deg.resize(n + 1);
        dis.assign(n + 1, -1E18);
    }
    void add(int x, int y, int w) {
        ver[x].push_back({y, w});
        ++deg[y];
    }
    int topsort(int s, int t) {
        queue<int> q;
        for (int i = 1; i <= n; i++) {
            if (deg[i] == 0) {
                q.push(i);
            }
        }
        dis[s] = 0;
        while (!q.empty()) {
            int x = q.front();
            q.pop();
            for (auto [y, w] : ver[x]) {
                dis[y] = max(dis[y], dis[x] + w);
                --deg[y];
                if (deg[y] == 0) {
                    q.push(y);
                }
            }
        }
        return dis[t];
    }
};

signed main() {
    int n, m;
    cin >> n >> m;
    DAG dag(n);
    for (int i = 1; i <= m; i++) {
        int x, y, w;
        cin >> x >> y >> w;
        dag.add(x, y, w);
    }
    
    int s, t;
    cin >> s >> t;
    cout << dag.topsort(s, t) << "\n";
}
