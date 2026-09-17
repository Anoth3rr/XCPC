/*
用途：倍增后缀数组与 height（lc）数组，支持 string 或可比较整数序列。
约定：rank[i] 为后缀 i 的名次；lc[r] 为 sa[r]、sa[r+1] 的 LCP。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
struct SA {

    int n = 0;
    vector<int> sa, rk, lc;

    SA() = default;

    template <class S> explicit SA(const S& s) {
        build(s);
    }

    template <class S> void build(const S& s) {
        n = s.size();
        sa.resize(n);
        rk.resize(n);
        lc.assign(n ? n - 1 : 0, 0);
        if (!n) return;
        iota(sa.begin(), sa.end(), 0);
        sort(sa.begin(), sa.end(), [&](int a, int b) { return s[a] < s[b]; });
        rk[sa[0]] = 0;
        for (int i = 1; i < n; ++i) rk[sa[i]] = rk[sa[i - 1]] + (s[sa[i]] != s[sa[i - 1]]);

        vector<int> a, b, c(n);
        a.reserve(n);
        for (int w = 1; rk[sa[n - 1]] < n - 1; w <<= 1) {
            a.clear();
            for (int i = max<int>(0, n - w); i < n; ++i) a.push_back(i);
            for (int i : sa)
                if (i >= w) a.push_back(i - w);
            fill(c.begin(), c.end(), 0);
            for (int i = 0; i < n; ++i) ++c[rk[i]];
            for (int i = 1; i < n; ++i) c[i] += c[i - 1];
            for (int i = n - 1; i >= 0; --i) sa[--c[rk[a[i]]]] = a[i];
            b = rk;
            rk[sa[0]] = 0;
            auto get = [&](int p) { return p < n ? b[p] : -1; };
            for (int i = 1; i < n; ++i) {
                int x = sa[i - 1], y = sa[i];
                rk[y] = rk[x] + (b[x] != b[y] || get(x + w) != get(y + w));
            }
        }

        for (int i = 0, j = 0; i < n; ++i) {
            if (!rk[i]) {
                j = 0;
                continue;
            }
            int p = sa[rk[i] - 1];
            while (i + j < n && p + j < n && s[i + j] == s[p + j]) ++j;
            lc[rk[i] - 1] = j;
            if (j) --j;
        }
    }
};
