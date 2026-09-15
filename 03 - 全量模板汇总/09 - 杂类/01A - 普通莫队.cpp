/*
用途：离线处理静态序列区间询问，约 O((n + q) sqrt(n))。
约定：下标从 1 开始，询问为闭区间；add/del 接收下标，get 返回当前答案。
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

    template <class A, class D, class G>
    auto solve(A add, D del, G get) {
        using R = decay_t<decltype(get())>;
        int m = q.size();
        vector<R> ans(m);
        if (!m) return ans;
        int z = max<int>(1, sqrt(m));
        int b = max<int>(1, n / z);
        sort(q.begin(), q.end(), [b](const Q& x, const Q& y) {
            int a = (x.l - 1) / b, c = (y.l - 1) / b;
            if (a != c) return a < c;
            return a & 1 ? x.r > y.r : x.r < y.r;
        });

        int l = 1, r = 0;
        for (auto [x, y, id] : q) {
            while (l > x) add(--l);
            while (r < y) add(++r);
            while (l < x) del(l++);
            while (r > y) del(r--);
            ans[id] = get();
        }
        return ans;
    }
};
