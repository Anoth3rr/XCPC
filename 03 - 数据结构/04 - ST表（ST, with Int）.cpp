template <class Int = ll>
struct ST {
    const int n, k;
    vector<vector<pair<Int, int>>> Max;
    vector<vector<pair<Int, int>>> Min;
    ST(int n) : n(n), k(__lg(n)) {
        Max.resize(k + 1, vector<pair<Int, int>>(n + 1));
        Min.resize(k + 1, vector<pair<Int, int>>(n + 1));
    }
    template <class Array>
    void init(Array &a) {
        for (int i = 1; i <= n; i++) {
            Max[0][i] = {a[i], -i};
            Min[0][i] = {a[i], -i};
        }
        for (int i = 0, t = 1; i < k; i++, t <<= 1) {
            const int T = n - (t << 1) + 1;
            for (int j = 1; j <= T; j++) {
                Max[i + 1][j] = max(Max[i][j], Max[i][j + t]);
                Min[i + 1][j] = min(Min[i][j], Min[i][j + t]);
            }
        }
    }
    pair<Int, int> getMax(int l, int r) {
        if (l > r) {
            swap(l, r);
        }
        int k = __lg(r - l + 1);
        return max(Max[k][l], Max[k][r - (1 << k) + 1]);
    }
    pair<Int, int> getMin(int l, int r) {
        if (l > r) {
            swap(l, r);
        }
        int k = __lg(r - l + 1);
        return min(Min[k][l], Min[k][r - (1 << k) + 1]);
    }
};