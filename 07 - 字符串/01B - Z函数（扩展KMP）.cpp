/*
用途：Z 函数、两串每个后缀与模式串的最长公共前缀。
约定：z[0]=|s|；askLCP(p, s)[i] 是 p 与 s[i..] 的 LCP 长度。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
struct ZFunction {

    template <class S> static vector<int> build(const S& s) {
        int n = s.size();
        vector<int> z(n);
        if (!n) return z;
        z[0] = n;
        for (int l = 0, r = 0, i = 1; i < n; ++i) {
            if (i < r) z[i] = min<int>(r - i, z[i - l]);
            while (i + z[i] < n && s[z[i]] == s[i + z[i]]) ++z[i];
            if (i + z[i] > r) l = i, r = i + z[i];
        }
        return z;
    }

    template <class P, class S> static vector<int> askLCP(const P& p, const S& s) {
        int n = s.size(), m = p.size();
        vector<int> a(n);
        if (!m) return a;
        auto z = build(p);
        for (int l = 0, r = 0, i = 0; i < n; ++i) {
            if (i < r) a[i] = min<int>(r - i, z[i - l]);
            while (a[i] < m && i + a[i] < n && p[a[i]] == s[i + a[i]]) ++a[i];
            if (i + a[i] > r) l = i, r = i + a[i];
        }
        return a;
    }

    template <class P, class S> static vector<int> ask(const P& p, const S& s) {
        int n = s.size(), m = p.size();
        if (!m) {
            vector<int> a(n + 1);
            iota(a.begin(), a.end(), 0);
            return a;
        }
        auto z = askLCP(p, s);
        vector<int> a;
        for (int i = 0; i < n; ++i)
            if (z[i] == m) a.push_back(i);
        return a;
    }
};
