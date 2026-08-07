/*
用途：模意义下维护区间和，支持区间乘、加、赋值；依赖自动取模类型 Z。
接口：build(a)、update(l,r,m,a) 表示 x<-m*x+a；updMul/updAdd/updSet/ask 均为闭区间。
复杂度：建树 O(n)，每次操作 O(log n)。
*/
template <class Z> struct SegTree {
    int n;
    vector<Z> s, m, a;

    explicit SegTree(int _n) : n(_n), s(4 * _n + 5), m(4 * _n + 5, Z(1)), a(4 * _n + 5) {
        assert(n > 0);
    }

    template <class V> void build(const V& v) {
        assert(v.size() > n);
        build(1, 1, n, v);
    }

    void update(int l, int r, Z x, Z y) {
        if (l > r) return;
        assert(1 <= l && r <= n);
        upd(1, 1, n, l, r, x, y);
    }
    void updMul(int l, int r, Z x) { update(l, r, x, Z(0)); }
    void updAdd(int l, int r, Z x) { update(l, r, Z(1), x); }
    void updSet(int l, int r, Z x) { update(l, r, Z(0), x); }
    Z ask(int l, int r) {
        if (l > r) return Z(0);
        assert(1 <= l && r <= n);
        return ask(1, 1, n, l, r);
    }

  private:
    template <class V> void build(int p, int l, int r, const V& v) {
        if (l == r) return void(s[p] = Z(v[l]));
        int q = (l + r) >> 1;
        build(p << 1, l, q, v), build(p << 1 | 1, q + 1, r, v);
        pull(p);
    }

    void apply(int p, int l, int r, Z x, Z y) {
        s[p] = s[p] * x + y * (r - l + 1);
        m[p] *= x;
        a[p] = a[p] * x + y;
    }

    void pull(int p) { s[p] = s[p << 1] + s[p << 1 | 1]; }

    void push(int p, int l, int r) {
        if (l == r) return;
        int q = (l + r) >> 1;
        apply(p << 1, l, q, m[p], a[p]);
        apply(p << 1 | 1, q + 1, r, m[p], a[p]);
        m[p] = Z(1), a[p] = Z(0);
    }

    void upd(int p, int l, int r, int x, int y, Z u, Z v) {
        if (x <= l && r <= y) return apply(p, l, r, u, v);
        push(p, l, r);
        int q = (l + r) >> 1;
        if (x <= q) upd(p << 1, l, q, x, y, u, v);
        if (y > q) upd(p << 1 | 1, q + 1, r, x, y, u, v);
        pull(p);
    }

    Z ask(int p, int l, int r, int x, int y) {
        if (x <= l && r <= y) return s[p];
        push(p, l, r);
        int q = (l + r) >> 1;
        if (y <= q) return ask(p << 1, l, q, x, y);
        if (x > q) return ask(p << 1 | 1, q + 1, r, x, y);
        return ask(p << 1, l, q, x, y) + ask(p << 1 | 1, q + 1, r, x, y);
    }
};
