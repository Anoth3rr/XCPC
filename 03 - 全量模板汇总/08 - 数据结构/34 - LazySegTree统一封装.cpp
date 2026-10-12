template<class Info, class Tag> struct LazySegTree {
    int n;
    vector<Info> info;
    vector<Tag> tag;
    vector<char> has; // 用于判断 tag[p] 是否有效

    LazySegTree(const vector<Info> &a) {
        n = a.size() - 1;
        info.assign(4 * n + 4, Info());
        tag.resize(4 * n + 4);
        has.assign(4 * n + 4, 0);
        build(1, 1, n, a);
    }
    void pointSet(int x, const Info &v) { // 单点 x 赋值为 v
        pointSet(1, 1, n, x, v);
    }
    void rangeApply(int l, int r, const Tag &v) { // 区间 [l, r] 修改
        rangeApply(1, 1, n, l, r, v);
    }
    Info rangeQuery(int l, int r) { // 查询区间 [l, r]
        return rangeQuery(1, 1, n, l, r);
    }

private:
    void build(int p, int l, int r, const vector<Info> &a) {
        if (l == r) return info[p] = a[l], void();
        int m = (l + r) / 2;
        build(p << 1, l, m, a);
        build(p << 1 | 1, m + 1, r, a);
        pull(p);
    }
    void pull(int p) {
        info[p] = info[p << 1] + info[p << 1 | 1];
    }
    void apply(int p, int l, int r, const Tag &v) {
        info[p].apply(v, r - l + 1);
        if (has[p]) tag[p].apply(v);
        else tag[p] = v, has[p] = 1;
    }
    void push(int p, int l, int r) {
        if (!has[p] || l == r) return;
        int m = (l + r) / 2;
        apply(p << 1, l, m, tag[p]);
        apply(p << 1 | 1, m + 1, r, tag[p]);
        has[p] = 0;
    }
    void pointSet(int p, int l, int r, int x, const Info &v) {
        if (l == r) return info[p] = v, has[p] = 0, void();
        push(p, l, r);
        int m = (l + r) / 2;
        if (x <= m) pointSet(p << 1, l, m, x, v);
        else pointSet(p << 1 | 1, m + 1, r, x, v);
        pull(p);
    }
    void rangeApply(int p, int l, int r, int x, int y, const Tag &v) {
        if (x <= l && r <= y) return apply(p, l, r, v);
        push(p, l, r);
        int m = (l + r) / 2;
        if (x <= m) rangeApply(p << 1, l, m, x, y, v);
        if (m < y) rangeApply(p << 1 | 1, m + 1, r, x, y, v);
        pull(p);
    }
    Info rangeQuery(int p, int l, int r, int x, int y) {
        if (x <= l && r <= y) return info[p];
        push(p, l, r);
        int m = (l + r) / 2;
        if (y <= m) return rangeQuery(p << 1, l, m, x, y);
        if (m < x) return rangeQuery(p << 1 | 1, m + 1, r, x, y);
        return rangeQuery(p << 1, l, m, x, y) + rangeQuery(p << 1 | 1, m + 1, r, x, y);
    }
};
