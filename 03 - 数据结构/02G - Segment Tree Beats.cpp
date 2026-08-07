/*
用途：Segment Tree Beats，支持区间 chmin、chmax、加法、和/最小/最大值查询。

约定：初始数组为 1-indexed。接口：build(a)、updMin、updMax、update、
askSum/askMin/askMax（均为闭区间）。均摊复杂度 O(log n)。
*/

struct SegTree {
    static constexpr int NI = numeric_limits<int>::lowest() / 4;
    static constexpr int PI = numeric_limits<int>::max() / 4;

    struct Node {
        int sum = 0, mx1 = NI, mx2 = NI, mn1 = PI, mn2 = PI, add = 0;
        int cmx = 0, cmn = 0;
    };

    int n = 0;
    vector<Node> tr;

    SegTree() = default;
    explicit SegTree(int n) {
        init(n);
    }

    void init(int _n) {
        assert(_n > 0);
        n = _n;
        tr.assign(4 * n + 5, Node{});
    }

    static int ls(int p) {
        return p << 1;
    }
    static int rs(int p) {
        return p << 1 | 1;
    }

    void pull(int p) {
        const Node &a = tr[ls(p)], &b = tr[rs(p)];
        Node &x = tr[p];
        x.sum = a.sum + b.sum;

        x.mx1 = max(a.mx1, b.mx1);
        x.cmx = 0;
        x.mx2 = NI;
        for (const Node *son : {&a, &b}) {
            if (son->mx1 == x.mx1) {
                x.cmx += son->cmx;
                x.mx2 = max(x.mx2, son->mx2);
            } else {
                x.mx2 = max(x.mx2, son->mx1);
            }
        }

        x.mn1 = min(a.mn1, b.mn1);
        x.cmn = 0;
        x.mn2 = PI;
        for (const Node *son : {&a, &b}) {
            if (son->mn1 == x.mn1) {
                x.cmn += son->cmn;
                x.mn2 = min(x.mn2, son->mn2);
            } else {
                x.mn2 = min(x.mn2, son->mn1);
            }
        }
        x.add = 0;
    }

    void setAdd(int p, int l, int r, int val) {
        Node &x = tr[p];
        x.sum += val * (r - l + 1);
        x.mx1 += val;
        if (x.mx2 != NI) x.mx2 += val;
        x.mn1 += val;
        if (x.mn2 != PI) x.mn2 += val;
        x.add += val;
    }

    void setMin(int p, int val) {
        Node &x = tr[p];
        assert(x.mx2 < val && val < x.mx1);
        x.sum += (val - x.mx1) * x.cmx;
        if (x.mn1 == x.mx1) x.mn1 = val;
        else if (x.mn2 == x.mx1) x.mn2 = val;
        x.mx1 = val;
    }

    void setMax(int p, int val) {
        Node &x = tr[p];
        assert(x.mn1 < val && val < x.mn2);
        x.sum += (val - x.mn1) * x.cmn;
        if (x.mx1 == x.mn1) x.mx1 = val;
        else if (x.mx2 == x.mn1) x.mx2 = val;
        x.mn1 = val;
    }

    void push(int p, int l, int r) {
        if (l == r) return;
        int mid = (l + r) >> 1;
        if (tr[p].add) {
            setAdd(ls(p), l, mid, tr[p].add);
            setAdd(rs(p), mid + 1, r, tr[p].add);
            tr[p].add = 0;
        }
        if (tr[ls(p)].mx1 > tr[p].mx1) setMin(ls(p), tr[p].mx1);
        if (tr[rs(p)].mx1 > tr[p].mx1) setMin(rs(p), tr[p].mx1);
        if (tr[ls(p)].mn1 < tr[p].mn1) setMax(ls(p), tr[p].mn1);
        if (tr[rs(p)].mn1 < tr[p].mn1) setMax(rs(p), tr[p].mn1);
    }

    template <class A>
    void build(int p, int l, int r, const A &a) {
        if (l == r) {
            int val = a[l];
            tr[p] = {val, val, NI, val, PI, 0, 1, 1};
            return;
        }
        int mid = (l + r) >> 1;
        build(ls(p), l, mid, a);
        build(rs(p), mid + 1, r, a);
        pull(p);
    }

    template <class A>
    void build(const A &a) {
        build(1, 1, n, a);
    }

    void updMin(int p, int l, int r, int ql, int qr, int val) {
        if (qr < l || r < ql || tr[p].mx1 <= val) return;
        if (ql <= l && r <= qr && tr[p].mx2 < val) {
            setMin(p, val);
            return;
        }
        push(p, l, r);
        int mid = (l + r) >> 1;
        updMin(ls(p), l, mid, ql, qr, val);
        updMin(rs(p), mid + 1, r, ql, qr, val);
        pull(p);
    }

    void updMax(int p, int l, int r, int ql, int qr, int val) {
        if (qr < l || r < ql || tr[p].mn1 >= val) return;
        if (ql <= l && r <= qr && tr[p].mn2 > val) {
            setMax(p, val);
            return;
        }
        push(p, l, r);
        int mid = (l + r) >> 1;
        updMax(ls(p), l, mid, ql, qr, val);
        updMax(rs(p), mid + 1, r, ql, qr, val);
        pull(p);
    }

    void update(int p, int l, int r, int ql, int qr, int val) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            setAdd(p, l, r, val);
            return;
        }
        push(p, l, r);
        int mid = (l + r) >> 1;
        update(ls(p), l, mid, ql, qr, val);
        update(rs(p), mid + 1, r, ql, qr, val);
        pull(p);
    }

    int askSum(int p, int l, int r, int ql, int qr) {
        if (qr < l || r < ql) return 0;
        if (ql <= l && r <= qr) return tr[p].sum;
        push(p, l, r);
        int mid = (l + r) >> 1;
        return askSum(ls(p), l, mid, ql, qr) + askSum(rs(p), mid + 1, r, ql, qr);
    }

    int askMin(int p, int l, int r, int ql, int qr) {
        if (qr < l || r < ql) return PI;
        if (ql <= l && r <= qr) return tr[p].mn1;
        push(p, l, r);
        int mid = (l + r) >> 1;
        return min(askMin(ls(p), l, mid, ql, qr), askMin(rs(p), mid + 1, r, ql, qr));
    }

    int askMax(int p, int l, int r, int ql, int qr) {
        if (qr < l || r < ql) return NI;
        if (ql <= l && r <= qr) return tr[p].mx1;
        push(p, l, r);
        int mid = (l + r) >> 1;
        return max(askMax(ls(p), l, mid, ql, qr), askMax(rs(p), mid + 1, r, ql, qr));
    }

    void updMin(int l, int r, int val) {
        if (l <= r) updMin(1, 1, n, l, r, val);
    }
    void updMax(int l, int r, int val) {
        if (l <= r) updMax(1, 1, n, l, r, val);
    }
    void update(int l, int r, int val) {
        if (l <= r) update(1, 1, n, l, r, val);
    }
    int askSum(int l, int r) {
        return askSum(1, 1, n, l, r);
    }
    int askMin(int l, int r) {
        return askMin(1, 1, n, l, r);
    }
    int askMax(int l, int r) {
        return askMax(1, 1, n, l, r);
    }
};
