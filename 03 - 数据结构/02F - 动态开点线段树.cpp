/*
用途：按需开点的区间加、区间和、单点赋值线段树。

接口与 02A 一致，区间和数组均为 1-indexed；适用于 n 很大、实际访问稀疏的场景。
注意查询非整段时也可能创建子节点，以保证懒标记语义正确。
*/
template <class T> struct Tag {
    T v = 0;

    void operator+=(const Tag<T> &o) {
        v += o.v;
    }

    bool has() const {
        return v != 0;
    }
};

template <class T> struct Info {
    T val = 0;
    int l = 1, r = 0;

    Info operator+(const Info<T> &o) const {
        if (l > r) return o;
        if (o.l > o.r) return *this;
        Info res;
        res.l = l;
        res.r = o.r;
        res.val = val + o.val;
        return res;
    }

    void operator+=(const Tag<T> &o) {
        if (l <= r) val += o.v * (r - l + 1);
    }

    bool hasSon() const {
        return l < r;
    }
};

template <class T> class SegTree {
  private:
    struct Node {
        Info<T> in;
        Tag<T> tag;
        int ls = 0, rs = 0;
    };

    vector<Node> tr;
    int n, rt;

    int node(int l, int r) {
        tr.push_back(Node());
        int x = tr.size() - 1;
        tr[x].in.l = l;
        tr[x].in.r = r;
        return x;
    }

    void apply(int x, const Tag<T> &v) {
        tr[x].in += v;
        tr[x].tag += v;
    }

    void push(int x) {
        if (!tr[x].in.hasSon()) return;

        int l = tr[x].in.l, r = tr[x].in.r;
        int mid = l + ((r - l) >> 1);

        if (!tr[x].ls) tr[x].ls = node(l, mid);
        if (!tr[x].rs) tr[x].rs = node(mid + 1, r);

        if (tr[x].tag.has()) {
            apply(tr[x].ls, tr[x].tag);
            apply(tr[x].rs, tr[x].tag);
            tr[x].tag = Tag<T>();
        }
    }

    void pull(int x) {
        tr[x].in = tr[tr[x].ls].in + tr[tr[x].rs].in;
    }

    void print(int x) {
        if (!x) return;
        cout << x << ":[" << tr[x].in.l << "," << tr[x].in.r << "],val:" << tr[x].in.val << ",tag:" << tr[x].tag.v << endl;
        print(tr[x].ls);
        print(tr[x].rs);
    }

    template <class A> void build(int x, A &dat) {
        int l = tr[x].in.l, r = tr[x].in.r;
        tr[x].tag = Tag<T>();

        if (l == r) {
            tr[x].in.val = dat[l];
            return;
        }

        int mid = l + ((r - l) >> 1);
        tr[x].ls = node(l, mid);
        tr[x].rs = node(mid + 1, r);

        build(tr[x].ls, dat);
        build(tr[x].rs, dat);
        pull(x);
    }

    void update(int x, int lq, int rq, const Tag<T> &v) {
        int l = tr[x].in.l, r = tr[x].in.r;
        if (rq < l || r < lq) return;

        if (lq <= l && r <= rq) {
            apply(x, v);
            return;
        }

        push(x);
        int mid = l + ((r - l) >> 1);

        if (lq <= mid) update(tr[x].ls, lq, rq, v);
        if (rq > mid) update(tr[x].rs, lq, rq, v);

        pull(x);
    }

    void modify(int x, int pos, T v) {
        int l = tr[x].in.l, r = tr[x].in.r;

        if (l == r) {
            tr[x].in.val = v;
            tr[x].tag = Tag<T>();
            return;
        }

        push(x);
        int mid = l + ((r - l) >> 1);

        if (pos <= mid)
            modify(tr[x].ls, pos, v);
        else
            modify(tr[x].rs, pos, v);

        pull(x);
    }

    Info<T> ask(int x, int lq, int rq) {
        int l = tr[x].in.l, r = tr[x].in.r;
        if (rq < l || r < lq) return Info<T>();

        if (lq <= l && r <= rq) return tr[x].in;

        push(x);
        int mid = l + ((r - l) >> 1);

        if (rq <= mid) return ask(tr[x].ls, lq, rq);
        if (lq > mid) return ask(tr[x].rs, lq, rq);
        return ask(tr[x].ls, lq, rq) + ask(tr[x].rs, lq, rq);
    }

  public:
    SegTree(int _n) : n(_n) {
        tr.push_back(Node());
        rt = node(1, n);
    }

    void print() {
        print(rt);
    }

    template <class A> void build(A &dat) {
        tr.clear();
        tr.push_back(Node());
        rt = node(1, n);
        build(rt, dat);
    }

    void update(int l, int r, Tag<T> v) {
        if (l > r) return;
        update(rt, l, r, v);
    }

    void modify(int pos, T v) {
        if (pos < 1 || pos > n) return;
        modify(rt, pos, v);
    }

    Info<T> ask(int l, int r) {
        if (l > r) return Info<T>();
        return ask(rt, l, r);
    }
};
