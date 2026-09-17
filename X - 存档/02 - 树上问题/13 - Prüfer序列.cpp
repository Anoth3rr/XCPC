struct PruferCode {
    static vector<int> encode(const vector<vector<int>> &g) {
        int n = g.size() - 1;
        if (n <= 2) return {};
        vector<int> d(n + 1);
        priority_queue<int, vector<int>, greater<int>> q;
        for (int i = 1; i <= n; ++i) {
            d[i] = g[i].size();
            if (d[i] == 1) q.push(i);
        }
        vector<int> p;
        p.reserve(n - 2);
        for (int k = 0; k < n - 2; ++k) {
            int u = q.top();
            q.pop();
            int v = 0;
            for (int x : g[u])
                if (d[x]) {
                    v = x;
                    break;
                }
            assert(v);
            p.push_back(v);
            d[u] = 0;
            if (--d[v] == 1) q.push(v);
        }
        return p;
    }

    static vector<array<int, 2>> decode(int n, const vector<int> &p) {
        assert(n >= 1 && p.size() == max<int>(0, n - 2));
        if (n == 1) return {};
        vector<int> d(n + 1, 1);
        for (int x : p) assert(1 <= x && x <= n), ++d[x];
        priority_queue<int, vector<int>, greater<int>> q;
        for (int i = 1; i <= n; ++i)
            if (d[i] == 1) q.push(i);
        vector<array<int, 2>> e;
        e.reserve(n - 1);
        for (int v : p) {
            int u = q.top();
            q.pop();
            e.push_back({u, v});
            --d[u];
            if (--d[v] == 1) q.push(v);
        }
        int u = q.top();
        q.pop();
        int v = q.top();
        e.push_back({u, v});
        return e;
    }
};
