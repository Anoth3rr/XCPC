struct BIT2d {
    int n, m;
    vector<vector<int>> w;

    BIT2d(int n, int m) : n(n), m(m), w(n + 1, vector<int>(m + 1)) {}
    void add(int u, int v, int x, int y, int p) {
        auto Add = [&](int x, int y, int p) {
            for (int i = x; i <= n; i += i & -i) {
                for (int j = y; j <= m; j += j & -j) {
                    w[i][j] += p;
                }
            }
        };
        x++, y++;
        Add(u, v, p), Add(x, v, -p);
        Add(x, y, p), Add(u, y, -p);
    }
    int ask(int x, int y) {
        int ans = 0;
        for (int i = x; i; i -= i & -i) {
            for (int j = y; j; j -= j & -j) {
                ans += w[i][j];
            }
        }
        return ans;
    }
};