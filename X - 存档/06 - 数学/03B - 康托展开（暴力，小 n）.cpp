/*
用途：求 1..n 排列的精确康托展开排名（从 1 开始），并支持反排名。
复杂度：O(n^2)，int 精确范围 n <= 20。大 n 且只需模意义排名时使用 03C。
*/
struct Cantor {
    static void check(const vector<int>& p) {
        int n = p.size();
        vector<char> vis(n + 1, false);
        for (int val : p) {
            assert(1 <= val && val <= n && !vis[val]);
            vis[val] = true;
        }
    }

    static int askRank(const vector<int>& p) {
        int n = p.size();
        assert(n <= 20);
        check(p);
        vector<int> fac(n + 1, 1);
        for (int i = 1; i <= n; ++i) fac[i] = fac[i - 1] * i;
        int rk = 1;
        for (int i = 0; i < n; ++i) {
            int cnt = 0;
            for (int j = i + 1; j < n; ++j) cnt += p[j] < p[i];
            rk += cnt * fac[n - 1 - i];
        }
        return rk;
    }

    static vector<int> askPerm(int n, int rk) {
        assert(0 <= n && n <= 20);
        vector<int> fac(n + 1, 1);
        for (int i = 1; i <= n; ++i) fac[i] = fac[i - 1] * i;
        assert(1 <= rk && rk <= fac[n]);
        --rk;
        vector<int> vis(n), p;
        iota(vis.begin(), vis.end(), 1);
        for (int rem = n; rem >= 1; --rem) {
            int id = rk / fac[rem - 1];
            rk %= fac[rem - 1];
            p.push_back(vis[id]);
            vis.erase(vis.begin() + id);
        }
        return p;
    }
};
