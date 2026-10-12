template<class Info> struct SegTreeIter {
    int n = 0;
    vector<Info> t;

    SegTreeIter(const vector<Info> &a) {
        n = a.size() - 1;
        t.assign(2 * n, Info());
        for (int i = 1; i <= n; i++) {
            t[n + i - 1] = a[i];
        }
        for (int p = n - 1; p; p--) {
            pull(p);
        }
    }
    void pull(int p) { t[p] = t[p << 1] + t[p << 1 | 1]; }
    void pointSet(int x, const Info &v) { // 单点 x 赋值为 v
        t[x += n - 1] = v;
        for (x >>= 1; x; x >>= 1) {
            pull(x);
        }
    }
    Info rangeQuery(int l, int r) const { // 查询闭区间 [l, r]
        Info u, v;
        for (l += n - 1, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) u = u + t[l++]; // 若信息不满足交换律（矩阵乘法、字符串拼接），必须注意顺序
            if (r & 1) v = t[--r] + v; // 若信息不满足交换律（矩阵乘法、字符串拼接），必须注意顺序
        }
        return u + v;
    }
};
