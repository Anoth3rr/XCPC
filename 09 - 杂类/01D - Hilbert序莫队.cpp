/*
用途：用 Hilbert 序替代分块排序的静态莫队，省去块长调参，常数通常更稳定。
约定：下标从 1 开始；add/del 接收下标，get 返回当前答案。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
struct Mo {
    struct Q {
        int l, r, id;
        int o;
    };

    int n, p = 1;
    vector<Q> q;

    explicit Mo(int n) : n(n) {
        int x = max<int>(1, n) - 1;
        p = 0;
        do ++p; while (x >>= 1);
    }

    static int ord(int x, int y, int p, int r = 0) {
        if (!p) return 0;
        int h = 1LL << (p - 1);
        int s = x < h ? (y < h ? 0 : 3) : (y < h ? 1 : 2);
        s = (s + r) & 3;
        static constexpr int d[] = {3, 0, 0, 1};
        int nr = (r + d[s]) & 3;
        int z = 1LL << (2 * p - 2);
        int w = ord(x & (h - 1), y & (h - 1), p - 1, nr);
        return s * z + (s == 1 || s == 2 ? w : z - w - 1);
    }

    void ask(int l, int r) {
        q.push_back({l, r, q.size(), ord(l - 1, r - 1, p)});
    }

    template <class A, class D, class G>
    auto solve(A add, D del, G get) {
        using R = decay_t<decltype(get())>;
        vector<R> ans(q.size());
        sort(q.begin(), q.end(), [](const Q& a, const Q& b) { return a.o < b.o; });
        int l = 1, r = 0;
        for (auto [x, y, id, o] : q) {
            while (l > x) add(--l);
            while (r < y) add(++r);
            while (l < x) del(l++);
            while (r > y) del(r--);
            ans[id] = get();
        }
        return ans;
    }
};
