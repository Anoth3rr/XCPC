template <class T> struct Tag {
    T v{};

    Tag &operator+=(const Tag &o) {
        v += o.v;
        return *this;
    }
};

template <class T> struct Info {
    T sum{};
    int len{};

    Info() = default;
    Info(T v, int len = 1) : sum(v), len(len) {}

    Info operator+(const Info &o) const {
        return {max(sum, o.sum), len + o.len};
    }

    Info &operator+=(const Tag<T> &tag) {
        sum += tag.v;
        return *this;
    }
};

template <class Info, class Tag> struct SegTree {
    int n;
    vector<Info> tr;
    vector<Tag> tag;
    vector<char> has;

    SegTree(int n) : n(n), tr(4 * n + 5), tag(4 * n + 5), has(4 * n + 5) {}

    void pull(int p) {
        tr[p] = tr[p << 1] + tr[p << 1 | 1];
    }

    void apply(int p, const Tag &v) {
        tr[p] += v;

        if (has[p])
            tag[p] += v;
        else
            tag[p] = v, has[p] = 1;
    }

    void push(int p) {
        if (!has[p]) return;

        apply(p << 1, tag[p]);
        apply(p << 1 | 1, tag[p]);

        tag[p] = Tag{};
        has[p] = 0;
    }

    template <class A> void build(int p, int l, int r, const A &a) {
        tag[p] = Tag{};
        has[p] = 0;

        if (l == r) {
            tr[p] = a[l];
            return;
        }

        int m = (l + r) >> 1;
        build(p << 1, l, m, a);
        build(p << 1 | 1, m + 1, r, a);
        pull(p);
    }

    template <class A> void build(const A &a) {
        build(1, 1, n, a);
    }

    void update(int p, int l, int r, int ql, int qr, const Tag &v) {
        if (ql <= l && r <= qr) {
            apply(p, v);
            return;
        }

        push(p);
        int m = (l + r) >> 1;

        if (ql <= m) update(p << 1, l, m, ql, qr, v);
        if (qr > m) update(p << 1 | 1, m + 1, r, ql, qr, v);

        pull(p);
    }

    void update(int l, int r, const Tag &v) {
        if (l > r) return;
        assert(1 <= l && r <= n);
        update(1, 1, n, l, r, v);
    }

    void modify(int p, int l, int r, int pos, const Info &v) {
        if (l == r) {
            tr[p] = v;
            tag[p] = Tag{};
            has[p] = 0;
            return;
        }

        push(p);
        int m = (l + r) >> 1;

        if (pos <= m)
            modify(p << 1, l, m, pos, v);
        else
            modify(p << 1 | 1, m + 1, r, pos, v);

        pull(p);
    }

    Info ask(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tr[p];

        push(p);
        int m = (l + r) >> 1;

        if (qr <= m) return ask(p << 1, l, m, ql, qr);
        if (ql > m) return ask(p << 1 | 1, m + 1, r, ql, qr);

        return ask(p << 1, l, m, ql, qr) + ask(p << 1 | 1, m + 1, r, ql, qr);
    }

    void modify(int pos, const Info &v) {
        modify(1, 1, n, pos, v);
    }

    Info ask(int l, int r) {
        if (l > r) return Info{};
        return ask(1, 1, n, l, r);
    }
};