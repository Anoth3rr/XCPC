/*
用途：离线处理单点赋值与区间第 k 小，复杂度 O((n+q) log V log n)。
约定：初始数组 a 为 1-indexed（a[0] 不使用）；modify/ask 的位置同样从 1 开始。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <class T>
struct KthModify {
    struct O {
        int t, p, l, r, k, id;
        T x;
    };
    struct E {
        int t, p, d, v, l, r, k, id;
    };

    vector<T> a, val, ans;
    vector<O> o;
    vector<int> bit;
    int n, qc = 0;

    explicit KthModify(vector<T> a) : a(std::move(a)) {
        assert(this->a.size() >= 2);
        n = this->a.size() - 1;
    }

    void modify(int p, T x) {
        assert(1 <= p && p <= n);
        o.push_back({0, p, 0, 0, 0, -1, x});
    }

    int ask(int l, int r, int k) {
        assert(1 <= l && l <= r && r <= n && 1 <= k && k <= r - l + 1);
        o.push_back({1, 0, l, r, k, qc, T{}});
        return qc++;
    }

    void bitAdd(int x, int v) {
        for (; x <= n; x += x & -x) bit[x] += v;
    }

    int bitAsk(int x) const {
        int r = 0;
        for (; x; x -= x & -x) r += bit[x];
        return r;
    }

    void dfs(int l, int r, vector<E> e) {
        bool h = false;
        for (auto& x : e) h |= x.t;
        if (!h) return;
        if (l == r) {
            for (auto& x : e) if (x.t) ans[x.id] = val[l];
            return;
        }
        int m = (l + r) / 2;
        vector<E> x, y;
        x.reserve(e.size());
        y.reserve(e.size());
        for (auto z : e) {
            if (!z.t) {
                if (z.v <= m) bitAdd(z.p, z.d), x.push_back(z);
                else y.push_back(z);
            } else {
                int c = bitAsk(z.r) - bitAsk(z.l - 1);
                if (c >= z.k) x.push_back(z);
                else z.k -= c, y.push_back(z);
            }
        }
        for (auto& z : e) if (!z.t && z.v <= m) bitAdd(z.p, -z.d);
        dfs(l, m, std::move(x));
        dfs(m + 1, r, std::move(y));
    }

    vector<T> solve() {
        val.assign(a.begin() + 1, a.end());
        for (auto& x : o) if (!x.t) val.push_back(x.x);
        sort(val.begin(), val.end());
        val.erase(unique(val.begin(), val.end()), val.end());
        auto id = [&](const T& x) -> int {
            return lower_bound(val.begin(), val.end(), x) - val.begin();
        };

        vector<E> e;
        e.reserve(n + 2 * o.size());
        vector<T> cur = a;
        for (int i = 1; i <= n; ++i) e.push_back({0, i, 1, id(cur[i]), 0, 0, 0, -1});
        for (auto& x : o) {
            if (!x.t) {
                e.push_back({0, x.p, -1, id(cur[x.p]), 0, 0, 0, -1});
                e.push_back({0, x.p, 1, id(x.x), 0, 0, 0, -1});
                cur[x.p] = x.x;
            } else {
                e.push_back({1, 0, 0, 0, x.l, x.r, x.k, x.id});
            }
        }
        ans.assign(qc, T{});
        bit.assign(n + 1, 0);
        if (qc) dfs(0, val.size() - 1, std::move(e));
        return ans;
    }
};
