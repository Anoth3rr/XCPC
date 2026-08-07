/*
用途：维护分段常值序列，支持半开区间 [l,r) 赋值、加法、第 k 小与幂和。
前提：只有当区间赋值足够随机、区间数长期很少时才使用；不存在可靠通用最坏复杂度。
自动取模：askSum<M> 令 M 为 MInt 即可让中间过程自动取模。
*/

template <class T, class I = int> struct ODT {
    struct Node {
        I l, r;
        mutable T v;
        bool operator<(const Node &o) const { return l < o.l; }
    };

    using It = typename set<Node>::iterator;
    I n = 0;
    set<Node> s;

    ODT() = default;
    explicit ODT(const vector<T> &a) { build(a); }

    void build(const vector<T> &a) {
        n = a.size();
        s.clear();
        for (I l = 0, r; l < n; l = r) {
            for (r = l + 1; r < n && a[r] == a[l]; ++r) {}
            s.insert({l, r, a[l]});
        }
    }

    It split(I p) {
        if (p >= n) return s.end();
        auto q = prev(s.upper_bound({p, p, T{}}));
        if (q->l == p) return q;
        auto [l, r, v] = *q;
        s.erase(q);
        s.insert({l, p, v});
        return s.insert({p, r, v}).first;
    }

    void update(I l, I r, const T &x) {
        auto e = split(r), b = split(l);
        s.erase(b, e);
        auto p = s.insert({l, r, x}).first;
        if (p != s.begin()) {
            auto q = prev(p);
            if (q->r == p->l && q->v == p->v) {
                I nl = q->l;
                s.erase(q);
                s.erase(p);
                p = s.insert({nl, r, x}).first;
            }
        }
        auto q = next(p);
        if (q != s.end() && p->r == q->l && p->v == q->v) {
            I nl = p->l;
            I nr = q->r;
            s.erase(q);
            s.erase(p);
            s.insert({nl, nr, x});
        }
    }

    void updAdd(I l, I r, const T &x) {
        auto e = split(r), b = split(l);
        for (auto p = b; p != e; ++p) p->v += x;
    }

    T askKth(I l, I r, I k) {
        auto e = split(r), b = split(l);
        vector<pair<T, I>> a;
        for (auto p = b; p != e; ++p) a.push_back({p->v, p->r - p->l});
        sort(a.begin(), a.end());
        for (auto [x, c] : a) if ((k -= c) <= 0) return x;
        throw out_of_range("ODT::askKth");
    }

    template <class M> M askSum(I l, I r, int e) {
        auto qp = [](M a, int b) {
            M z = 1;
            for (; b; b >>= 1, a *= a) if (b & 1) z *= a;
            return z;
        };
        auto q = split(r), b = split(l);
        M z = 0;
        for (auto p = b; p != q; ++p) z += qp(M(p->v), e) * M(p->r - p->l);
        return z;
    }
};
