/*
用途：离线处理区间询问与单点修改，n、q 同阶时约 O(n^(5/3))。
约定：下标从 1 开始。每读到一次修改先调用 modify，再加入后续询问。
apply/undo 接收修改编号和当前 [l,r]；若修改点在区间内，应先 del，再改值，最后 add。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
struct Mo {
    struct Q {
        int l, r, t, id;
    };

    int n, c = 0;
    vector<Q> q;

    explicit Mo(int n) : n(n) {}

    void modify() { ++c; }

    void ask(int l, int r) {
        q.push_back({l, r, c, q.size()});
    }

    template <class A, class D, class P, class U, class G>
    auto solve(A add, D del, P ap, U ud, G get) {
        using R = decay_t<decltype(get())>;
        int m = q.size();
        vector<R> ans(m);
        if (!m) return ans;
        int b = max<int>(1, pow(max(n, m), 2.0 / 3.0));
        sort(q.begin(), q.end(), [b](const Q& x, const Q& y) {
            int a = (x.l - 1) / b, c = (y.l - 1) / b;
            if (a != c) return a < c;
            int d = (x.r - 1) / b, e = (y.r - 1) / b;
            if (d != e) return a & 1 ? d > e : d < e;
            return (a ^ d) & 1 ? x.t > y.t : x.t < y.t;
        });

        int l = 1, r = 0, t = 0;
        for (auto [x, y, z, id] : q) {
            while (t < z) ap(++t, l, r);
            while (t > z) ud(t--, l, r);
            while (l > x) add(--l);
            while (r < y) add(++r);
            while (l < x) del(l++);
            while (r > y) del(r--);
            ans[id] = get();
        }
        return ans;
    }
};
