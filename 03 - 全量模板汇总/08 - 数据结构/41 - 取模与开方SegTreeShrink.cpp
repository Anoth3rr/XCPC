template<class T> T isqrt(T n) { // n >= 0
    T x = sqrt(n); // 兼容 __int128 需要更换为 sqrtl
    while (x + 1 <= n / (x + 1)) ++x;
    while (x && x > n / x) --x;
    return x;
}

struct SegTreeShrink {
    int n;
    vector<i64> sum, mx;

    SegTreeShrink(const vector<i64> &a) : n(a.size() - 1), sum(4 * n + 4), mx(4 * n + 4) {
        build(1, 1, n, a);
    }

    void pointSet(int x, i64 v) { pointSet(1, 1, n, x, v); }
    void rangeMod(int l, int r, i64 v) { rangeMod(1, 1, n, l, r, v); }
    void rangeSqrt(int l, int r) { rangeSqrt(1, 1, n, l, r); }
    i64 rangeSum(int l, int r) { return rangeSum(1, 1, n, l, r); }

private:
    void build(int p, int l, int r, const vector<i64> &a) {
        if (l == r) return sum[p] = mx[p] = a[l], void();
        int m = (l + r) / 2;
        build(p << 1, l, m, a);
        build(p << 1 | 1, m + 1, r, a);
        pull(p);
    }
    void pull(int p) {
        sum[p] = sum[p << 1] + sum[p << 1 | 1];
        mx[p] = max(mx[p << 1], mx[p << 1 | 1]);
    }
    void pointSet(int p, int l, int r, int x, i64 v) {
        if (l == r) return sum[p] = mx[p] = v, void();
        int m = (l + r) / 2;
        if (x <= m) pointSet(p << 1, l, m, x, v);
        else pointSet(p << 1 | 1, m + 1, r, x, v);
        pull(p);
    }
    void rangeMod(int p, int l, int r, int x, int y, i64 v) {
        if (mx[p] < v) return; // 整段取模后不变
        if (l == r) return sum[p] %= v, mx[p] = sum[p], void();
        int m = (l + r) / 2;
        if (x <= m) rangeMod(p << 1, l, m, x, y, v);
        if (m < y) rangeMod(p << 1 | 1, m + 1, r, x, y, v);
        pull(p);
    }
    void rangeSqrt(int p, int l, int r, int x, int y) {
        if (mx[p] <= 1) return; // 0、1 开方后不变
        if (l == r) return sum[p] = mx[p] = isqrt(sum[p]), void();
        int m = (l + r) / 2;
        if (x <= m) rangeSqrt(p << 1, l, m, x, y);
        if (m < y) rangeSqrt(p << 1 | 1, m + 1, r, x, y);
        pull(p);
    }
    i64 rangeSum(int p, int l, int r, int x, int y) {
        if (x <= l && r <= y) return sum[p];
        int m = (l + r) / 2;
        i64 ans = 0;
        if (x <= m) ans += rangeSum(p << 1, l, m, x, y);
        if (m < y) ans += rangeSum(p << 1 | 1, m + 1, r, x, y);
        return ans;
    }
};
