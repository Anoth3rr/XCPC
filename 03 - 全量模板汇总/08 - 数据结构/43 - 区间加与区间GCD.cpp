struct RangeAddGcd {
    int n;
    vector<i64> d, bit, seg;

    RangeAddGcd(const vector<i64> &a) : n(a.size() - 1), d(n + 1), bit(n + 1), seg(2 * n) {
        for (int i = 1; i <= n; i++) {
            d[i] = a[i] - (i == 1 ? 0 : a[i - 1]);
            bit[i] += d[i];
            int j = i + (i & -i);
            if (j <= n) bit[j] += bit[i];
            seg[n + i - 1] = gcd(d[i], 0LL);
        }
        for (int p = n - 1; p; p--) {
            seg[p] = gcd(seg[p << 1], seg[p << 1 | 1]);
        }
    }

    void rangeAdd(int l, int r, i64 v) { // a[l..r] += v
        changeD(l, v);
        changeD(r + 1, -v);
    }
    void pointSet(int x, i64 v) { // a[x] = v
        rangeAdd(x, x, v - pointValue(x));
    }
    i64 pointValue(int x) const { return prefixSum(x); }
    i64 rangeGcd(int l, int r) const {
        return gcd(pointValue(l), rangeGcdDiff(l + 1, r));
    }

private:
    void bitAdd(int x, i64 v) {
        for (; x <= n; x += x & -x) bit[x] += v;
    }
    i64 prefixSum(int x) const {
        i64 ans = 0;
        for (; x; x -= x & -x) ans += bit[x];
        return ans;
    }
    void changeD(int x, i64 v) { // d[x] += v
        if (x > n) return;
        d[x] += v;
        bitAdd(x, v);

        int p = n + x - 1;
        seg[p] = gcd(d[x], 0LL);
        for (p >>= 1; p; p >>= 1) {
            seg[p] = gcd(seg[p << 1], seg[p << 1 | 1]);
        }
    }
    i64 rangeGcdDiff(int l, int r) const {
        if (l > r) return 0;
        i64 ans = 0;
        for (l += n - 1, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) ans = gcd(ans, seg[l++]);
            if (r & 1) ans = gcd(ans, seg[--r]);
        }
        return ans;
    }
};
