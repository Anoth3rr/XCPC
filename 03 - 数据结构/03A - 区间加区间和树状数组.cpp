/*
用途：双树状数组，支持区间加和区间求和。

下标和区间均为 1-indexed 闭区间。
接口：update(l,r,v)、ask(x)、askSum(l,r)。每次 O(log n)。
*/
template <class T> struct BIT {
    int n = 0;
    vector<T> b1, b2;

    BIT() = default;
    explicit BIT(int n) {
        init(n);
    }

    void init(int _n) {
        n = _n;
        b1.assign(n + 1, T{});
        b2.assign(n + 1, T{});
    }

    void mod0(vector<T> &bit, int x, const T &v) {
        for (; x <= n; x += x & -x) bit[x] += v;
    }

    T ask0(const vector<T> &bit, int x) const {
        T res{};
        for (; x > 0; x -= x & -x) res += bit[x];
        return res;
    }

    void update(int l, int r, const T &v) {
        if (l > r) return;
        assert(1 <= l && r <= n);
        mod0(b1, l, v);
        mod0(b2, l, v * (l - 1));
        if (r < n) {
            mod0(b1, r + 1, -v);
            mod0(b2, r + 1, -v * r);
        }
    }

    T ask(int x) const {
        x = min(x, n);
        return ask0(b1, x) * x - ask0(b2, x);
    }

    T askSum(int l, int r) const {
        if (l > r) return T{};
        assert(1 <= l && r <= n);
        return ask(r) - ask(l - 1);
    }
};
