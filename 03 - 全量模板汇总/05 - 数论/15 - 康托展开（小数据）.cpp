namespace Cantor {
    constexpr int N = 10;
    vector<int> f(N + 1);
    void init() {
        f[0] = f[1] = 1;
        for (int i = 2; i <= n; ++i) f[i] = f[i - 1] * i;
    }
    int rank(const vector<int> &p) {
        int n = p.size(), rk = 1;
        for (int i = 0; i < n; ++i) {
            int cnt = 0;
            for (int j = i + 1; j < n; ++j) cnt += p[j] < p[i];
            rk += cnt * f[n - 1 - i];
        }
        return rk;
    }

    vector<int> perm(int n, int rk) {
        vector<int> f(n + 1, 1);
        for (int i = 1; i <= n; ++i) f[i] = f[i - 1] * i;
        --rk;
        vector<int> vis(n), p;
        iota(vis.begin(), vis.end(), 1);
        for (int rem = n; rem >= 1; --rem) {
            int id = rk / f[rem - 1];
            rk %= f[rem - 1];
            p.push_back(vis[id]);
            vis.erase(vis.begin() + id);
        }
        return p;
    }
}; // namespace Cantor