/*
用途：静态幂等最值 RMQ；64 位块内单调栈状压，块间 Sparse Table。
约定：数组与查询均为 0-indexed，ask/askPos 使用闭区间 [l,r]；相等时返回最左位置。
复杂度：字长模型下 O(n) 预处理、O(1) 查询，空间 O(n)。Cmp 默认为求最小值。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/

using u64 = uint64_t;

template <class T, class Cmp = less<T>> struct LinearRMQ {
    static constexpr int B = 64;

    int n = 0, nb = 0;
    Cmp cmp;
    vector<T> a;
    vector<int> pre, suf;
    vector<u64> stk;
    vector<vector<int>> st;

    LinearRMQ() = default;
    explicit LinearRMQ(const vector<T>& a, Cmp cmp = Cmp()) : cmp(move(cmp)) { build(a); }

    int best(int x, int y) const {
        if (x < 0) return y;
        if (y < 0) return x;
        if (cmp(a[y], a[x])) return y;
        if (cmp(a[x], a[y])) return x;
        return min(x, y);
    }

    void build(const vector<T>& v) {
        a = v;
        n = a.size();
        if (!n) {
            nb = 0, pre.clear(), suf.clear(), stk.clear(), st.clear();
            return;
        }
        nb = (n + B - 1) / B;
        pre.resize(n), suf.resize(n), stk.resize(n);
        vector<int> b(nb);
        for (int q = 0; q < nb; ++q) {
            int l = q * B, r = min<int>(n, l + B);
            u64 s = 0;
            for (int i = l; i < r; ++i) {
                pre[i] = i == l ? i : best(pre[i - 1], i);
                while (s) {
                    int k = 63 - __builtin_clzll(s);
                    if (!cmp(a[i], a[l + k])) break;
                    s ^= u64(1) << k;
                }
                s |= u64(1) << (i - l);
                stk[i] = s;
            }
            for (int i = r; i-- > l;) suf[i] = i + 1 == r ? i : best(i, suf[i + 1]);
            b[q] = pre[r - 1];
        }
        int lg = nb == 1 ? 0 : 31 - __builtin_clz(nb);
        st.assign(lg + 1, vector<int>(nb));
        st[0] = move(b);
        for (int k = 1; k <= lg; ++k)
            for (int i = 0; i + (1LL << k) <= nb; ++i)
                st[k][i] = best(st[k - 1][i], st[k - 1][i + (1LL << (k - 1))]);
    }

    int askPos(int l, int r) const {
        assert(0 <= l && l <= r && r < n);
        int x = l / B, y = r / B;
        if (x == y) {
            u64 s = stk[r] >> (l - x * B);
            return l + __builtin_ctzll(s);
        }
        int z = best(suf[l], pre[r]);
        int ql = x + 1, qr = y - 1;
        if (ql <= qr) {
            int k = 31 - __builtin_clz(qr - ql + 1);
            z = best(z, best(st[k][ql], st[k][qr - (1LL << k) + 1]));
        }
        return z;
    }

    const T& ask(int l, int r) const { return a[askPos(l, r)]; }
};
