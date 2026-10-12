struct HopcroftKarp {
    int n, m;
    vector<array<int, 2>> ver;
    vector<int> l, r;

    HopcroftKarp(int n, int m) : n(n), m(m) { // 左右半部
        l.assign(n, -1);
        r.assign(m, -1);
    }
    void add(int x, int y) {
        x--, y--; // 这个板子是 0-idx 的
        ver.push_back({x, y});
    }
    int work() {
        vector<int> adj(ver.size());

        mt19937 rgen(chrono::steady_clock::now().time_since_epoch().count());
        shuffle(ver.begin(), ver.end(), rgen); // 随机化防卡

        vector<int> deg(n + 1);
        for (auto &[u, v] : ver) {
            deg[u]++;
        }
        for (int i = 1; i <= n; i++) {
            deg[i] += deg[i - 1];
        }
        for (auto &[u, v] : ver) {
            adj[--deg[u]] = v;
        }

        int ans = 0;
        vector<int> a, p, q(n);
        while (true) {
            a.assign(n, -1), p.assign(n, -1);

            int t = 0;
            for (int i = 0; i < n; i++) {
                if (l[i] == -1) {
                    q[t++] = a[i] = p[i] = i;
                }
            }

            bool match = false;
            for (int i = 0; i < t; i++) {
                int x = q[i];
                if (~l[a[x]]) continue;

                for (int j = deg[x]; j < deg[x + 1]; j++) {
                    int y = adj[j];
                    if (r[y] == -1) {
                        while (~y) {
                            r[y] = x;
                            swap(l[x], y);
                            x = p[x];
                        }
                        match = true;
                        ++ans;
                        break;
                    }
                    if (p[r[y]] == -1) {
                        q[t++] = y = r[y];
                        p[y] = x;
                        a[y] = a[x];
                    }
                }
            }
            if (!match) break;
        }
        return ans;
    }
    vector<array<int, 2>> answer() {
        vector<array<int, 2>> ans;
        for (int i = 0; i < n; i++) {
            if (~l[i]) {
                ans.push_back({i, l[i]});
            }
        }
        return ans;
    }
};

signed main() {
    int n1, n2, m;
    cin >> n1 >> n2 >> m;
    HopcroftKarp flow(n1, n2);
    while (m--) {
        int x, y;
        cin >> x >> y;
        flow.add(x, y);
    }

    cout << flow.work() << "\n";

    auto match = flow.answer();
    for (auto [u, v] : match) {
        cout << u << " " << v << "\n";
    }
}
