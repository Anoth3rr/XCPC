/*
用途：add 容易而 del 困难时的静态区间询问，约 O((n + q) sqrt(n))。
回调：clear 清空状态；save 返回快照；rollback(s) 回到快照；brute(l,r) 独立暴力同块询问。
约定：下标从 1 开始，add 只加入下标，get 返回当前答案。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
struct Mo {
    struct Q {
        int l, r, id;
    };

    int n;
    vector<Q> q;

    explicit Mo(int n) : n(n) {}

    void ask(int l, int r) {
        q.push_back({l, r, q.size()});
    }

    template <class A, class C, class S, class R, class G, class B>
    auto solve(A add, C clr, S sav, R ud, G get, B bf) {
        using V = decay_t<decltype(get())>;
        int m = q.size();
        vector<V> ans(m);
        if (!m) return ans;
        int b = max<int>(1, sqrt(n));
        sort(q.begin(), q.end(), [b](const Q& x, const Q& y) {
            int a = (x.l - 1) / b, c = (y.l - 1) / b;
            if (a != c) return a < c;
            return x.r < y.r;
        });

        for (int p = 0; p < m;) {
            int k = (q[p].l - 1) / b, z = p;
            while (z < m && (q[z].l - 1) / b == k) ++z;
            int e = min<int>(n + 1, (k + 1) * b + 1);
            clr();
            int r = e - 1;
            for (int i = p; i < z; ++i) {
                auto [l, qr, id] = q[i];
                if (qr < e) {
                    ans[id] = bf(l, qr);
                    continue;
                }
                while (r < qr) add(++r);
                auto s = sav();
                for (int j = e - 1; j >= l; --j) add(j);
                ans[id] = get();
                ud(s);
            }
            p = z;
        }
        return ans;
    }
};
