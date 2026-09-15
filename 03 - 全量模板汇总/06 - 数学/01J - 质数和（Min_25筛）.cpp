/*
用途：求不超过 n 的所有质数之和；这是 Min_25 筛的基础质数幂和筛法。
复杂度：约 O(n^(3/4)/log n) 时间、O(sqrt(n)) 空间，适合 n 远大于可直接筛的范围。
T 默认 i128 做精确计算，也可传 MInt 自动取模；三角和先整除 2，兼容偶数模数。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/

using i128 = __int128_t;

using ld = long double;

template <class T = i128> struct Min25 {
    int n = 0, s = 0;
    vector<int> w;
    vector<int> id1, id2, pr;
    vector<T> g, pre;

    int id(int x) const { return x <= s ? id1[x] : id2[n / x]; }

    static T tri(int x) {
        int a = x, b = x + 1;
        if (a & 1) b >>= 1;
        else a >>= 1;
        return T(a) * T(b);
    }

    T askSum(int _n) {
        assert(_n >= 0);
        n = _n;
        if (n < 2) return T{};
        s = sqrtl(static_cast<ld>(n));
        while ((s + 1) <= n / (s + 1)) ++s;
        while (s > n / s) --s;

        vector<char> vis(s + 1);
        pr.clear();
        pre.assign(1, T{});
        for (int i = 2; i <= s; ++i) {
            if (!vis[i]) {
                pr.push_back(i);
                pre.push_back(pre.back() + T(i));
            }
            for (int p : pr) {
                if (i * p > s) break;
                vis[i * p] = true;
                if (i % p == 0) break;
            }
        }

        w.clear(), g.clear();
        id1.assign(s + 1, 0), id2.assign(s + 1, 0);
        for (int l = 1, r; l <= n; l = r + 1) {
            int v = n / l;
            r = n / v;
            int k = w.size();
            w.push_back(v);
            g.push_back(tri(v) - T(1));
            if (v <= s) id1[v] = k;
            else id2[n / v] = k;
        }

        for (int i = 0; i < pr.size(); ++i) {
            int p = pr[i];
            if (p > n / p) break;
            int pp = p * p;
            for (int j = 0; j < w.size() && w[j] >= pp; ++j)
                g[j] -= T(p) * (g[id(w[j] / p)] - pre[i]);
        }
        return g[id(n)];
    }
};
