/*
用途：不带懒标记的通用线段树。

Info 需要默认构造，并定义 Info operator+(const Info&)；数组与区间均为 1-indexed 闭区间。
支持 build、单点 modify、区间 ask 和整段 ask，均为 O(log n)。
*/
template <class T> class SegTree {
  private:
    vector<T> val;
    int n = 0;

    int ls(int x) {
        return x << 1;
    }
    int rs(int x) {
        return x << 1 | 1;
    }

    template <class A> void build(int x, int l, int r, const A &a) {
        if (l == r) {
            val[x] = a[l];
            return;
        }
        int mid = (l + r) >> 1;
        build(ls(x), l, mid, a);
        build(rs(x), mid + 1, r, a);
        val[x] = val[ls(x)] + val[rs(x)];
    }

    void modify(int x, int l, int r, int p, const T &v) {
        if (l == r) {
            val[x] = v;
            return;
        }
        int mid = (l + r) >> 1;
        if (p <= mid) modify(ls(x), l, mid, p, v);
        else modify(rs(x), mid + 1, r, p, v);
        val[x] = val[ls(x)] + val[rs(x)];
    }

    T ask(int x, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return val[x];
        int mid = (l + r) >> 1;
        if (qr <= mid) return ask(ls(x), l, mid, ql, qr);
        if (ql > mid) return ask(rs(x), mid + 1, r, ql, qr);
        return ask(ls(x), l, mid, ql, qr) + ask(rs(x), mid + 1, r, ql, qr);
    }

  public:
    SegTree(int _n = 0) {
        if (_n) init(_n);
    }

    void init(int _n) {
        n = _n;
        val.assign(4 * n + 5, T());
    }

    template <class A> void build(const A &a) {
        build(1, 1, n, a);
    }

    void modify(int p, const T &v) {
        modify(1, 1, n, p, v);
    }

    T ask(int l, int r) {
        return ask(1, 1, n, l, r);
    }

    T ask() {
        return val[1];
    }
};
