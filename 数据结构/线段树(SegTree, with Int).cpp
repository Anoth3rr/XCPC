template <class Int = ll>
struct Tag {
    Int v = 0;
    void operator+=(const Tag<Int> &o) {
        v += o.v;
    }
    bool check() {
        return v != 0;
    }
};

template <class Int = ll>
struct Info {
    Int val = 0;
    int l, r;
    Info operator+(const Info<Int> &o) const {
        Info res;
        res.l = l;
        res.r = o.r;
        res.val = val + o.val;
        return res;
    }
    void operator+=(const Tag<Int> &o) {
        val += o.v * (r - l + 1);
    }
    bool check() {
        return l != r;
    }
};

template <class Int = ll>
class SegTree {
  private:
    vector<Info<Int>> info;
    vector<Tag<Int>> tag;
    int n;

    int ls(int x) { return x << 1; }
    int rs(int x) { return x << 1 | 1; }

    void print(int x, int l, int r) {
        cout << x << ":[" << l << "," << r << "],val:" << info[x].val << ",tag:" << tag[x].v << "\n";
        if (l == r)
            return;
        int mid = l + r >> 1;
        print(ls(x), l, mid);
        print(rs(x), mid + 1, r);
    }

    template <class Array>
    void build(int x, int l, int r, Array &data) {
        if (l == r) {
            info[x].l = l;
            info[x].r = r;
            info[x].val = data[l];
            return;
        }
        int mid = (l + r) >> 1;
        build(ls(x), l, mid, data);
        build(rs(x), mid + 1, r, data);
        info[x] = info[ls(x)] + info[rs(x)];
    }

    void push_down(int x) {
        if (tag[x].check() && info[x].check()) {
            info[ls(x)] += tag[x];
            info[rs(x)] += tag[x];
            tag[ls(x)] += tag[x];
            tag[rs(x)] += tag[x];
            tag[x] = {0};
        }
    }

    void update(int x, int l, int r, int lq, int rq, Tag<Int> v) {
        if (rq < l || lq > r)
            return;
        if (lq <= l && r <= rq) {
            info[x] += v;
            tag[x] += v;
            return;
        }
        push_down(x);
        int mid = (l + r) >> 1;
        update(ls(x), l, mid, lq, rq, v);
        update(rs(x), mid + 1, r, lq, rq, v);
        info[x] = info[ls(x)] + info[rs(x)];
    }

    void modify(int x, int l, int r, int pos, Int v) {
        if (r < pos || l > pos)
            return;
        if (l == r && l == pos) {
            info[x].val = v;
            return;
        }
        int mid = (l + r) >> 1;
        modify(ls(x), l, mid, pos, v);
        modify(rs(x), mid + 1, r, pos, v);
        info[x] = info[ls(x)] + info[rs(x)];
    }

    Info<Int> ask(int x, int l, int r, int lq, int rq) {
        if (rq < l || lq > r)
            return {Info<Int>()};
        if (lq <= l && r <= rq)
            return info[x];
        push_down(x);
        int mid = (l + r) >> 1;
        auto ans = ask(ls(x), l, mid, lq, rq) + ask(rs(x), mid + 1, r, lq, rq);
        return ans;
    }

  public:
    SegTree(int n_) : n(n_), info(4 * n_ + 1), tag(4 * n_ + 1) {}

    void print() {
        print(1, 1, n);
    }

    template <class Array>
    void build(Array &data) {
        build(1, 1, n, data);
    }

    void update(int l, int r, Tag<Int> v) {
        update(1, 1, n, l, r, v);
    }

    void modify(int pos, Int v) {
        modify(1, 1, n, pos, v);
    }

    Info<Int> ask(int l, int r) {
        return ask(1, 1, n, l, r);
    }
};