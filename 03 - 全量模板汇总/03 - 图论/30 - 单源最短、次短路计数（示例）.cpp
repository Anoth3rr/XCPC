const int N = 2e5 + 7, M = 1e6 + 7;
int n, m, s, e; int d[N][2], v[N][2]; // 0 代表最短路， 1 代表次短路
Z num[N][2];

void Clear() {
    for (int i = 1; i <= n; ++ i) h[i] = edge[i] = 0;
    tot = 0;
    for (int i = 1; i <= n; ++ i) num[i][0] = num[i][1] = v[i][0] = v[i][1] = 0;
    for (int i = 1; i <= n; ++ i) d[i][0] = d[i][1] = INF;
}

int ver[M], ne[M], h[N], edge[M], tot;
void add(int x, int y, int w) {
    ver[++ tot] = y, ne[tot] = h[x], h[x] = tot;
    edge[tot] = w;
}

void dji() {
    priority_queue<PIII, vector<PIII>, greater<PIII> > q; q.push({0, s, 0});
    num[s][0] = 1; d[s][0] = 0;
    while (!q.empty()) {
        auto [dis, x, type] = q.top(); q.pop();
        if (v[x][type]) continue; v[x][type] = 1;
        for (int i = h[x]; i; i = ne[i]) {
            int y = ver[i], w = dis + edge[i];
            if (d[y][0] > w) {
                d[y][1] = d[y][0], num[y][1] = num[y][0];
                    // 如果找到新的最短路，将原有的最短路数据转化为次短路
                q.push({d[y][1], y, 1});
                d[y][0] = w, num[y][0] = num[x][type];
                q.push({d[y][0], y, 0});
            }
            else if (d[y][0] == w) num[y][0] += num[x][type];
            else if (d[y][1] > w) {
                d[y][1] = w, num[y][1] = num[x][type];
                q.push({d[y][1], y, 1});
            }
            else if (d[y][1] == w) num[y][1] += num[x][type];
        }
    }
}
void Solve() {
    cin >> n >> m >> s >> e;
    Clear(); //多组样例务必完全清空
    for (int i = 1; i <= m; ++ i) {
        int x, y, w; cin >> x >> y; w = 1;
        add(x, y, w), add(y, x, w);
    }
    dji();
    Z ans = num[e][0];
    if (d[e][1] == d[e][0] + 1) {
        ans += num[e][1]; // 只有在次短路满足条件时才计算（距离恰好比最短路大1）
    }
    cout << ans.val() << endl;
}
