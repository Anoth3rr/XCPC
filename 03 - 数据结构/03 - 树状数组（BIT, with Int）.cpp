/*
用途：一维树状数组，默认维护单点加、前缀和与区间和。

接口（均为 1-indexed）：modify(p,x)、ask(p)、ask(l,r)、askKth(k)。
update(l,r,x) 是差分用法：区间加、单点查询；不要与区间和语义混用。
askKth(k) 返回最小的 p 使前缀和 >= k；要求各点权非负，非法 k 返回 n + 1。
*/
template <class T>
struct BIT {
    int n = 0;
    vector<T> bit;

    BIT() = default;
    explicit BIT(int n) {
        init(n);
    }

    void init(int _n) {
        n = _n;
        bit.assign(n + 1, T{});
    }

    template <class A>
    void build(const A &a) {
        fill(bit.begin(), bit.end(), T{});
        for (int i = 1; i <= n; ++i) {
            bit[i] += a[i];
            int j = i + (i & -i);
            if (j <= n) bit[j] += bit[i];
        }
    }

    void modify(int x, const T &v) {
        assert(1 <= x && x <= n);
        for (; x <= n; x += x & -x) bit[x] += v;
    }

    void update(int l, int r, const T &v) {
        if (l > r) return;
        assert(1 <= l && r <= n);
        modify(l, v);
        if (r < n) modify(r + 1, -v);
    }

    T ask(int x) const {
        x = min(x, n);
        T res{};
        for (; x > 0; x -= x & -x) res += bit[x];
        return res;
    }

    T ask(int l, int r) const {
        if (l > r) return T{};
        assert(1 <= l && r <= n);
        return ask(r) - ask(l - 1);
    }

    int askKth(const T &k) const {
        if (!(k > T{})) return 1;
        int pos = 0;
        T pre{};
        int stp = 1;
        while ((stp << 1) <= n) stp <<= 1;
        for (; stp; stp >>= 1) {
            int nxt = pos + stp;
            if (nxt <= n && pre + bit[nxt] < k) {
                pre += bit[nxt];
                pos = nxt;
            }
        }
        return pos + 1;
    }
};
