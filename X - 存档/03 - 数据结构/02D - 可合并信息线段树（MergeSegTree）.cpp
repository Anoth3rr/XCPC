struct Info {
    int val{}, len{};
    Info() = default;
    Info(int v, int len = 1) : val(v), len(len) {}

    Info operator+(const Info &o) const {
        return {max(val, o.val), len + o.len};
    }
};

struct SegTree {
    int n;
    vector<Info> tr;

    SegTree(int n) : n(n), tr(4 * n + 5) {}

    template <class A> void build(int p, int l, int r, const A &a) {
        if (l == r) return tr[p] = a[l], void();

        int m = (l + r) >> 1;
        build(p << 1, l, m, a);
        build(p << 1 | 1, m + 1, r, a);
        tr[x] = tr[p << 1] + tr[p << 1 | 1];
    }

    template <class A> void build(const A &a) {
        build(1, 1, n, a);
    }

    void modify(int p, int l, int r, int pos, const Info &v) {
        if (l == r) return tr[p] = v, void();

        int m = (l + r) >> 1;
        if (pos <= m)
            modify(p << 1, l, m, pos, v);
        else
            modify(p << 1 | 1, m + 1, r, pos, v);
        tr[x] = tr[p << 1] + tr[p << 1 | 1];
    }

    void modify(int pos, const Info &v) {
        modify(1, 1, n, pos, v);
    }

    Info ask(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tr[p];
        int m = (l + r) >> 1;
        if (qr <= mid) return ask(x << 1, l, m, ql, qr);
        if (ql > mid) return ask(x << 1 | 1, m + 1, r, ql, qr);
        return ask(x << 1, l, m, ql, qr) + ask(x << 1 | 1, m + 1, r, ql, qr);
    }

    Info ask(int l, int r) {
        if (l > r) return Info{};
        return ask(1, 1, n, l, r);
    }
};