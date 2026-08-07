/*
用途：静态区间最值 ST 表，查询最大/最小值及其最左位置。

约定：数组和查询均为 1-indexed 闭区间；askMax/askMin 自动交换 l、r。
仅适用于 max/min/gcd 等幂等操作，区间和请用“04B - 可结合区间查询（Disjoint Sparse Table）”。
预处理 O(n log n)，每次查询 O(1)。
性能：为返回最左位置，表项额外保存 int 下标；极限内存不足时可只将下标改为 i32。
*/
template <class T>
struct ST {
    int n = 0, lg = 0;
    vector<vector<pair<T, int>>> mx, mn;

    ST() = default;
    explicit ST(int n) {
        init(n);
    }

    void init(int _n) {
        assert(_n > 0);
        n = _n;
        lg = __lg(n);
        mx.assign(lg + 1, vector<pair<T, int>>(n + 1));
        mn.assign(lg + 1, vector<pair<T, int>>(n + 1));
    }

    template <class A>
    void build(const A &a) {
        assert(n > 0);
        for (int i = 1; i <= n; ++i) {
            mx[0][i] = {a[i], -i};  // 值相同时优先较小下标。
            mn[0][i] = {a[i], i};
        }
        for (int k = 1; k <= lg; ++k) {
            int len = 1 << k;
            int h = len >> 1;
            for (int l = 1; l + len - 1 <= n; ++l) {
                mx[k][l] = max(mx[k - 1][l], mx[k - 1][l + h]);
                mn[k][l] = min(mn[k - 1][l], mn[k - 1][l + h]);
            }
        }
    }

    pair<T, int> askMax(int l, int r) const {
        if (l > r) swap(l, r);
        assert(1 <= l && r <= n);
        int k = __lg(r - l + 1);
        auto res = max(mx[k][l], mx[k][r - (1 << k) + 1]);
        return {res.first, -res.second};
    }

    pair<T, int> askMin(int l, int r) const {
        if (l > r) swap(l, r);
        assert(1 <= l && r <= n);
        int k = __lg(r - l + 1);
        return min(mn[k][l], mn[k][r - (1 << k) + 1]);
    }
};
