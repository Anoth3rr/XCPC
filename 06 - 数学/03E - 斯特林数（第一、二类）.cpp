/*
用途：预处理无符号第一类、第二类 Stirling 数。
复杂度：O(n^2)。
*/
template <class Z> struct Stirling {
    int n = 0;
    vector<vector<Z>> a, b;

    Stirling() = default;

    explicit Stirling(int m) {
        init(m);
    }

    void init(int m) {
        assert(m >= 0);
        n = m;
        a.assign(n + 1, vector<Z>(n + 1));
        b.assign(n + 1, vector<Z>(n + 1));
        a[0][0] = b[0][0] = 1;
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= i; ++j) {
                a[i][j] = a[i - 1][j - 1] + Z(i - 1) * a[i - 1][j];
                b[i][j] = b[i - 1][j - 1] + Z(j) * b[i - 1][j];
            }
        }
    }

    Z stir1(int x, int k) const {
        assert(0 <= x && x <= n && 0 <= k && k <= x);
        return (x - k) & 1 ? -a[x][k] : a[x][k];
    }
};
