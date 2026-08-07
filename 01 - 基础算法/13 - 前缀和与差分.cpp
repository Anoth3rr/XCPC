/*
用途：静态一维/二维区间和与一维区间加差分。
约定：输入数组下标均为 0-indexed，ask / update 的区间均为闭区间 [l,r]。
复杂度：前缀和预处理 O(n) 或 O(nm)、查询 O(1)；差分每次更新 O(1)、build O(n)。
*/
template <class T> struct PrefixSum {
    vector<T> pre;

    PrefixSum() = default;

    explicit PrefixSum(const vector<T>& a) {
        build(a);
    }

    void build(const vector<T>& a) {
        pre.assign(a.size() + 1, T{});
        for (int i = 0; i < a.size(); ++i) {
            pre[i + 1] = pre[i] + a[i];
        }
    }

    T ask(int l, int r) const {
        assert(0 <= l && l <= r && r + 1 < pre.size());
        return pre[r + 1] - pre[l];
    }
};

template <class T> struct Prefix2D {
    vector<vector<T>> pre;

    Prefix2D() = default;

    explicit Prefix2D(const vector<vector<T>>& a) {
        build(a);
    }

    void build(const vector<vector<T>>& a) {
        int n = a.size();
        int m = n == 0 ? 0 : a[0].size();
        for (const auto& v : a) assert(v.size() == m);

        pre.assign(n + 1, vector<T>(m + 1, T{}));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                pre[i + 1][j + 1] = a[i][j] + pre[i][j + 1] + pre[i + 1][j] - pre[i][j];
            }
        }
    }

    T ask(int x1, int y1, int x2, int y2) const {
        assert(0 <= x1 && x1 <= x2 && 0 <= y1 && y1 <= y2 &&
               x2 + 1 < pre.size() && y2 + 1 < pre[0].size());
        return pre[x2 + 1][y2 + 1] - pre[x1][y2 + 1] - pre[x2 + 1][y1] + pre[x1][y1];
    }
};

template <class T> struct Diff {
    vector<T> d;

    Diff() = default;

    explicit Diff(const vector<T>& a) {
        build(a);
    }

    void build(const vector<T>& a) {
        d.assign(a.size() + 1, T{});
        for (int i = 0; i < a.size(); ++i) {
            d[i] += a[i];
            d[i + 1] -= a[i];
        }
    }

    void update(int l, int r, const T& x) {
        assert(0 <= l && l <= r && r + 1 < d.size());
        d[l] += x;
        d[r + 1] -= x;
    }

    vector<T> ask() const {
        assert(!d.empty());
        vector<T> res(d.size() - 1);
        T cur{};
        for (int i = 0; i < res.size(); ++i) {
            cur += d[i];
            res[i] = cur;
        }
        return res;
    }
};
