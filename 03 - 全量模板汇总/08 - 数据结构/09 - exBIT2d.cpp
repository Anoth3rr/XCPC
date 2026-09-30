struct exBIT2d {
    int n, m;
    vector<vector<int>> b1, b2, b3, b4;

    exBIT2d(int n, int m) : n(n), m(m), b1(n + 1, vector<int>(m + 1)), b2(n + 1, vector<int>(m + 1)), b3(n + 1, vector<int>(m + 1)), b4(n + 1, vector<int>(m + 1)) {}
    void add(int u, int v, int x, int y, int p) {
        auto Add = [&](int u, int v, int p) {
            for (int i = u; i <= n; i += i & -i) {
                for (int j = v; j <= m; j += j & -j) {
                    b1[i][j] += p;
                    b2[i][j] += p * (u - 1);
                    b3[i][j] += p * (v - 1);
                    b4[i][j] += p * (u - 1) * (v - 1);
                }
            }
        };
        x++, y++;
        Add(u, v, p), Add(x, v, -p);
        Add(x, y, p), Add(u, y, -p);
    }
    int ask(int u, int v, int x, int y) {
        auto ask = [&](int u, int v) {
            int ans = 0;
            for (int i = u; i; i -= i & -i) {
                for (int j = v; j; j -= j & -j) {
                    ans += b1[i][j] * u * v;
                    ans -= b2[i][j] * v;
                    ans -= b3[i][j] * u;
                    ans += b4[i][j];
                }
            }
            return ans;
        };
        return ask(x, y) - ask(u - 1, y) - ask(x, v - 1) + ask(u - 1, v - 1);
    }
};