/*
用途：区间加、区间最小值/最大值、单点赋值的懒标记线段树。

约定：初始数组和所有区间均为 1-indexed 闭区间。
接口：build(a)、update(l, r, Tag<T>{delta})、modify(pos, value)、ask(l, r).
查询结果的 mx / mn 分别为区间最大值 / 最小值。
*/
template <class T>
struct Tag {
    T v = T{};

    Tag &operator+=(const Tag &o) {
        v += o.v;
        return *this;
    }

    bool has() const {
        return v != T{};
    }
};

template <class T>
struct Info {
    T mx = numeric_limits<T>::lowest();
    T mn = numeric_limits<T>::max();
    int l = 1, r = 0;

    bool askEmp() const {
        return l > r;
    }

    bool hasSon() const {
        return l < r;
    }

    Info operator+(const Info &o) const {
        if (askEmp()) return o;
        if (o.askEmp()) return *this;
        return {max(mx, o.mx), min(mn, o.mn), l, o.r};
    }

    void operator+=(const Tag<T> &tag) {
        if (!askEmp()) mx += tag.v, mn += tag.v;
    }
};

template <class T>
class SegTree {
  private:
    int n;
    vector<Info<T>> val;
    vector<Tag<T>> tag;

    static int ls(int x) {
        return x << 1;
    }
    static int rs(int x) {
        return x << 1 | 1;
    }

    void apply(int x, const Tag<T> &val) {
        val[x] += val;
        tag[x] += val;
    }

    void pull(int x) {
        val[x] = val[ls(x)] + val[rs(x)];
    }

    void push(int x) {
        if (!val[x].hasSon() || !tag[x].has()) return;
        apply(ls(x), tag[x]);
        apply(rs(x), tag[x]);
        tag[x] = Tag<T>{};
    }

    template <class A>
    void build(int x, int l, int r, const A &a) {
        val[x].l = l;
        val[x].r = r;
        tag[x] = Tag<T>{};
        if (l == r) {
            val[x].mx = val[x].mn = a[l];
            return;
        }
        int mid = (l + r) >> 1;
        build(ls(x), l, mid, a);
        build(rs(x), mid + 1, r, a);
        pull(x);
    }

    void update(int x, int l, int r, int ql, int qr, const Tag<T> &val) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            apply(x, val);
            return;
        }
        push(x);
        int mid = (l + r) >> 1;
        update(ls(x), l, mid, ql, qr, val);
        update(rs(x), mid + 1, r, ql, qr, val);
        pull(x);
    }

    void modify(int x, int l, int r, int pos, const T &val) {
        if (l == r) {
            val[x].mx = val[x].mn = val;
            tag[x] = Tag<T>{};
            return;
        }
        push(x);
        int mid = (l + r) >> 1;
        if (pos <= mid) modify(ls(x), l, mid, pos, val);
        else modify(rs(x), mid + 1, r, pos, val);
        pull(x);
    }

    Info<T> ask(int x, int l, int r, int ql, int qr) {
        if (qr < l || r < ql) return {};
        if (ql <= l && r <= qr) return val[x];
        push(x);
        int mid = (l + r) >> 1;
        return ask(ls(x), l, mid, ql, qr) + ask(rs(x), mid + 1, r, ql, qr);
    }

  public:
    explicit SegTree(int n) : n(n), val(4 * n + 5), tag(4 * n + 5) {
        assert(n > 0);
    }

    template <class A>
    void build(const A &a) {
        build(1, 1, n, a);
    }

    void update(int l, int r, const Tag<T> &val) {
        if (l > r) return;
        assert(1 <= l && r <= n);
        update(1, 1, n, l, r, val);
    }

    void modify(int pos, const T &val) {
        assert(1 <= pos && pos <= n);
        modify(1, 1, n, pos, val);
    }

    Info<T> ask(int l, int r) {
        if (l > r) return {};
        assert(1 <= l && r <= n);
        return ask(1, 1, n, l, r);
    }
};
