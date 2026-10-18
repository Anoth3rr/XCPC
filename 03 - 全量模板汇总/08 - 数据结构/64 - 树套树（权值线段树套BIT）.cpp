struct SegBIT {
    int n = 1;
    vector<vector<int>> p;
    vector<BIT> w;

    SegBIT(int m, vector<pii> a) {
        while (n < m) n <<= 1;
        p.resize(2 * n);
        sort(a.begin(), a.end());
        for (auto [x, y] : a) {
            for (int u = y + n - 1; u; u >>= 1) {
                if (p[u].empty() || p[u].back() != x) p[u].push_back(x);
            }
        }
        w.reserve(2 * n);
        for (auto &v : p) w.emplace_back(v.size());
    }

    void add(int x, int y, int v) {
        for (int u = y + n - 1; u; u >>= 1) {
            int k = lower_bound(p[u].begin(), p[u].end(), x) - p[u].begin() + 1;
            w[u].add(k, v);
        }
    }

    int ask(int u, int l, int r) {
        int x = lower_bound(p[u].begin(), p[u].end(), l) - p[u].begin() + 1;
        int y = upper_bound(p[u].begin(), p[u].end(), r) - p[u].begin();
        return w[u].ask(x, y);
    }

    int count(int l, int r, int k) {
        int res = 0;
        for (int u = n, v = n + k; u < v; u >>= 1, v >>= 1) {
            if (u & 1) res += ask(u++, l, r);
            if (v & 1) res += ask(--v, l, r);
        }
        return res;
    }

    int kth(int l, int r, int k) {
        int u = 1;
        while (u < n) {
            int c = ask(u << 1, l, r);
            if (k <= c) {
                u <<= 1;
            } else {
                k -= c;
                u = u << 1 | 1;
            }
        }
        return u - n + 1;
    }
};
