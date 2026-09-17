template <class Info, class Tag> class LazySegTree {
  private:
    int n = 0;
    vector<Info> val;
    vector<Tag> tag;

    int ls(int x) {
        return x << 1;
    }
    int rs(int x) {
        return x << 1 | 1;
    }

    void apply(int x, const Tag &v) {
        val[x] += v;
        tag[x] += v;
    }

    void pull(int x) {
        val[x] = val[ls(x)] + val[rs(x)];
    }

    void push(int x) {
        if (!tag[x].has()) return;
        apply(ls(x), tag[x]);
        apply(rs(x), tag[x]);
        tag[x] = Tag();
    }

    template <class A> void build(int x, int l, int r, const A &a) {
        tag[x] = Tag();
        if (l == r) {
            val[x] = a[l];
            return;
        }
        int mid = (l + r) >> 1;
        build(ls(x), l, mid, a);
        build(rs(x), mid + 1, r, a);
        pull(x);
    }

    void update(int x, int l, int r, int ql, int qr, const Tag &v) {
        if (ql <= l && r <= qr) {
            apply(x, v);
            return;
        }
        push(x);
        int mid = (l + r) >> 1;
        if (ql <= mid) update(ls(x), l, mid, ql, qr, v);
        if (qr > mid) update(rs(x), mid + 1, r, ql, qr, v);
        pull(x);
    }

    void modify(int x, int l, int r, int p, const Info &v) {
        if (l == r) {
            val[x] = v;
            tag[x] = Tag();
            return;
        }
        push(x);
        int mid = (l + r) >> 1;
        if (p <= mid)
            modify(ls(x), l, mid, p, v);
        else
            modify(rs(x), mid + 1, r, p, v);
        pull(x);
    }

    Info ask(int x, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return val[x];
        push(x);
        int mid = (l + r) >> 1;
        if (qr <= mid) return ask(ls(x), l, mid, ql, qr);
        if (ql > mid) return ask(rs(x), mid + 1, r, ql, qr);
        return ask(ls(x), l, mid, ql, qr) + ask(rs(x), mid + 1, r, ql, qr);
    }

  public:
    explicit LazySegTree(int _n = 0) {
        if (_n) init(_n);
    }

    void init(int _n) {
        assert(_n > 0);
        n = _n;
        val.assign(4 * n + 5, Info());
        tag.assign(4 * n + 5, Tag());
    }

    template <class A> void build(const A &a) {
        assert(n > 0);
        build(1, 1, n, a);
    }

    void update(int l, int r, const Tag &v) {
        if (l > r) return;
        assert(1 <= l && r <= n);
        update(1, 1, n, l, r, v);
    }

    void modify(int p, const Info &v) {
        assert(1 <= p && p <= n);
        modify(1, 1, n, p, v);
    }

    Info ask(int l, int r) {
        if (l > r) return Info();
        assert(1 <= l && r <= n);
        return ask(1, 1, n, l, r);
    }

    Info ask() const {
        return val[1];
    }
};
