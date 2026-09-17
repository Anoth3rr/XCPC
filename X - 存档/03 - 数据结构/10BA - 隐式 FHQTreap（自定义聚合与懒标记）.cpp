template <class Info, class Tag, class Op> struct SeqTreap {
    struct Node {
        int l = 0, r = 0, sz = 0;
        unsigned pri = 0;
        Info val{}, fwd{}, bwd{};
        Tag tag{};
        bool rev = false, has = false;
    };

    vector<Node> tr = {Node{}};
    int rt = 0;
    Op op;
    mt19937 rng{static_cast<mt19937::result_type>(chrono::steady_clock::now().time_since_epoch().count())};

    explicit SeqTreap(Op ac = Op()) : op(ac) {}

    void reserve(int n) {
        tr.reserve(n + 1);
    }

    int sz(int p) const {
        return p ? tr[p].sz : 0;
    }
    Info fwd(int p) const {
        return p ? tr[p].fwd : Info::unit();
    }
    Info bwd(int p) const {
        return p ? tr[p].bwd : Info::unit();
    }

    int node(const Info &v) {
        tr.push_back(Node{});
        int p = tr.size() - 1;
        tr[p].val = tr[p].fwd = tr[p].bwd = v;
        tr[p].sz = 1;
        tr[p].pri = rng();
        return p;
    }

    void pull(int p) {
        if (!p) return;
        tr[p].sz = sz(tr[p].l) + 1 + sz(tr[p].r);
        tr[p].fwd = (fwd(tr[p].l) + tr[p].val) + fwd(tr[p].r);
        tr[p].bwd = (bwd(tr[p].r) + tr[p].val) + bwd(tr[p].l);
    }

    void setTag(int p, const Tag &t) {
        if (!p) return;
        op.apply(tr[p].val, t);
        op.apply(tr[p].fwd, t);
        op.apply(tr[p].bwd, t);
        if (tr[p].has)
            tr[p].tag = op.join(tr[p].tag, t);
        else
            tr[p].tag = t, tr[p].has = true;
    }

    void flip(int p) {
        if (!p) return;
        swap(tr[p].l, tr[p].r);
        swap(tr[p].fwd, tr[p].bwd);
        tr[p].rev = !tr[p].rev;
    }

    void push(int p) {
        if (!p) return;
        if (tr[p].has) {
            setTag(tr[p].l, tr[p].tag);
            setTag(tr[p].r, tr[p].tag);
            tr[p].has = false;
        }
        if (tr[p].rev) {
            flip(tr[p].l);
            flip(tr[p].r);
            tr[p].rev = false;
        }
    }

    // a 为前 k 个元素，b 为其余元素。
    void split(int p, int k, int &a, int &b) {
        if (!p) {
            a = b = 0;
            return;
        }
        push(p);
        if (sz(tr[p].l) >= k) {
            b = p;
            split(tr[p].l, k, a, tr[p].l);
            pull(b);
        } else {
            a = p;
            split(tr[p].r, k - sz(tr[p].l) - 1, tr[p].r, b);
            pull(a);
        }
    }

    int merge(int a, int b) {
        if (!a || !b) return a | b;
        if (tr[a].pri < tr[b].pri) {
            push(a);
            tr[a].r = merge(tr[a].r, b);
            pull(a);
            return a;
        }
        push(b);
        tr[b].l = merge(a, tr[b].l);
        pull(b);
        return b;
    }

    int askSz() const {
        return sz(rt);
    }

    void insert(int pos, const Info &v) {
        assert(0 <= pos && pos <= askSz());
        int a, b;
        split(rt, pos, a, b);
        rt = merge(merge(a, node(v)), b);
    }

    void erase(int l, int r) {
        assert(0 <= l && l <= r && r <= askSz());
        int a, b, c;
        split(rt, r, a, c);
        split(a, l, a, b);
        rt = merge(a, c);
    }

    void modify(int pos, const Info &v) {
        assert(0 <= pos && pos < askSz());
        int a, b, c;
        split(rt, pos + 1, a, c);
        split(a, pos, a, b);
        push(b);
        tr[b].val = tr[b].fwd = tr[b].bwd = v;
        pull(b);
        rt = merge(merge(a, b), c);
    }

    void update(int l, int r, const Tag &t) {
        assert(0 <= l && l <= r && r <= askSz());
        int a, b, c;
        split(rt, r, a, c);
        split(a, l, a, b);
        setTag(b, t);
        rt = merge(merge(a, b), c);
    }

    void reverse(int l, int r) {
        assert(0 <= l && l <= r && r <= askSz());
        int a, b, c;
        split(rt, r, a, c);
        split(a, l, a, b);
        flip(b);
        rt = merge(merge(a, b), c);
    }

    // 将当前 [l,r) 剪下，插到删除后位置 p 之前。
    void move(int l, int r, int p) {
        assert(0 <= l && l <= r && r <= askSz());
        assert(0 <= p && p <= askSz() - (r - l));
        int a, b, c;
        split(rt, r, a, c);
        split(a, l, a, b);
        rt = merge(a, c);
        split(rt, p, a, c);
        rt = merge(merge(a, b), c);
    }

    Info ask(int l, int r) {
        assert(0 <= l && l <= r && r <= askSz());
        int a, b, c;
        split(rt, r, a, c);
        split(a, l, a, b);
        Info res = fwd(b);
        rt = merge(merge(a, b), c);
        return res;
    }
};
