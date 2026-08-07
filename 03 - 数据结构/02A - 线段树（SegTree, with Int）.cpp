/*
用途：区间加、区间和、单点赋值的懒标记线段树。

约定：初始数组和所有区间均为 1-indexed 闭区间。
接口：build(a)、update(l, r, Tag<T>{delta})、modify(pos, value)、ask(l, r).val。
T 需要支持 +、*、-= 等常规数值运算，通常使用 int。
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
    T val = T{};
    int l = 1, r = 0;  // 空信息满足 l > r。

    bool askEmp() const {
        return l > r;
    }

    bool hasSon() const {
        return l < r;
    }

    Info operator+(const Info &o) const {
        if (askEmp()) return o;
        if (o.askEmp()) return *this;
        return {val + o.val, l, o.r};
    }

    void operator+=(const Tag<T> &tag) {
        if (!askEmp()) val += tag.v * (r - l + 1);
    }
};

template <class T>
class SegTree {
  private:
    int n;
    vector<Info<T>> tr;
    vector<Tag<T>> tag;

    static int ls(int x) {
        return x << 1;
    }
    static int rs(int x) {
        return x << 1 | 1;
    }

    void apply(int x, const Tag<T> &v) {
        tr[x] += v;
        tag[x] += v;
    }

    void pull(int x) {
        tr[x] = tr[ls(x)] + tr[rs(x)];
    }

    void push(int x) {
        if (!tr[x].hasSon() || !tag[x].has()) return;
        apply(ls(x), tag[x]);
        apply(rs(x), tag[x]);
        tag[x] = Tag<T>{};
    }

    template <class A>
    void build(int x, int l, int r, const A &a) {
        tr[x].l = l;
        tr[x].r = r;
        tag[x] = Tag<T>{};
        if (l == r) {
            tr[x].val = a[l];
            return;
        }
        int mid = (l + r) >> 1;
        build(ls(x), l, mid, a);
        build(rs(x), mid + 1, r, a);
        pull(x);
    }

    void update(int x, int l, int r, int ql, int qr, const Tag<T> &v) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            apply(x, v);
            return;
        }
        push(x);
        int mid = (l + r) >> 1;
        update(ls(x), l, mid, ql, qr, v);
        update(rs(x), mid + 1, r, ql, qr, v);
        pull(x);
    }

    void modify(int x, int l, int r, int pos, const T &v) {
        if (l == r) {
            tr[x].val = v;
            tag[x] = Tag<T>{};
            return;
        }
        push(x);
        int mid = (l + r) >> 1;
        if (pos <= mid) modify(ls(x), l, mid, pos, v);
        else modify(rs(x), mid + 1, r, pos, v);
        pull(x);
    }

    Info<T> ask(int x, int l, int r, int ql, int qr) {
        if (qr < l || r < ql) return {};
        if (ql <= l && r <= qr) return tr[x];
        push(x);
        int mid = (l + r) >> 1;
        return ask(ls(x), l, mid, ql, qr) + ask(rs(x), mid + 1, r, ql, qr);
    }

  public:
    explicit SegTree(int n) : n(n), tr(4 * n + 5), tag(4 * n + 5) {
        assert(n > 0);
    }

    template <class A>
    void build(const A &a) {
        build(1, 1, n, a);
    }

    void update(int l, int r, const Tag<T> &v) {
        if (l > r) return;
        assert(1 <= l && r <= n);
        update(1, 1, n, l, r, v);
    }

    void modify(int pos, const T &v) {
        assert(1 <= pos && pos <= n);
        modify(1, 1, n, pos, v);
    }

    Info<T> ask(int l, int r) {
        if (l > r) return {};
        assert(1 <= l && r <= n);
        return ask(1, 1, n, l, r);
    }
};
