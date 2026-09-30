struct BIT2d {
    int n, m;
    vector<vector<int>> w;

    BIT2d(int n, int m) : n(n), m(m), w(n + 1, vector<int>(m + 1)) {}
    void add(int x, int y, int p) { 
        for (int i = x; i <= n; i += i & -i) {
            for (int j = y; j <= m; j += j & -j) {
                w[i][j] += p;
            }
        }
    }
    int ask(int u, int v, int x, int y) {
        auto sum = [&](int x, int y) {
            int ans = 0;
            for (int i = x; i; i -= i & -i) {
                for (int j = y; j; j -= j & -j) {
                    ans += w[i][j];
                }
            }
            return ans;
        };
        return sum(x, y) - sum(u - 1, y) - sum(x, v - 1) + sum(u - 1, v - 1);
    }
};