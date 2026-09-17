using i128 = __int128_t;

struct LiChaoTree {
    static constexpr int INF = numeric_limits<int>::max();

    struct Line {
        int m = 0, b = INF;
        i128 eval(int x) const {
            return i128(m) * x + b;
        }
    };

    struct Node {
        Line f;
        int ch[2]{};
    };

    int lo, hi;
    vector<Node> tr;
    int rt = 0;

    LiChaoTree(int l, int r) : lo(l), hi(r), tr(1) {
        assert(lo <= hi);
    }

    int node() {
        tr.push_back(Node{});
        return tr.size() - 1;
    }

    int add0(int p, int l, int r, Line f) {
        if (!p) p = node();
        int mid = l + (r - l) / 2;
        bool bl = f.eval(l) < tr[p].f.eval(l);
        bool bm = f.eval(mid) < tr[p].f.eval(mid);
        if (bm) swap(f, tr[p].f);
        if (l == r) return p;
        if (bl != bm)
            tr[p].ch[0] = add0(tr[p].ch[0], l, mid, f);
        else
            tr[p].ch[1] = add0(tr[p].ch[1], mid + 1, r, f);
        return p;
    }

    int add1(int p, int l, int r, int ql, int qr, Line f) {
        if (qr < l || r < ql) return p;
        if (ql <= l && r <= qr) {
            return add0(p, l, r, f);
        }
        if (!p) p = node();
        int mid = l + (r - l) / 2;
        tr[p].ch[0] = add1(tr[p].ch[0], l, mid, ql, qr, f);
        tr[p].ch[1] = add1(tr[p].ch[1], mid + 1, r, ql, qr, f);
        return p;
    }

    i128 ask0(int p, int l, int r, int x) const {
        if (!p) return i128(INF);
        i128 ans = tr[p].f.eval(x);
        if (l == r) return ans;
        int mid = l + (r - l) / 2;
        if (x <= mid) return min(ans, ask0(tr[p].ch[0], l, mid, x));
        return min(ans, ask0(tr[p].ch[1], mid + 1, r, x));
    }

    void add(int m, int b) {
        rt = add0(rt, lo, hi, {m, b});
    }

    void addSeg(int m, int b, int ql, int qr) {
        ql = max(ql, lo);
        qr = min(qr, hi);
        if (ql <= qr) rt = add1(rt, lo, hi, ql, qr, {m, b});
    }

    int ask(int x) const {
        assert(lo <= x && x <= hi);
        i128 ans = ask0(rt, lo, hi, x);
        assert(ans >= numeric_limits<int>::lowest() && ans <= numeric_limits<int>::max());
        return ans;
    }
};
