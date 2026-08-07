/*
用途：SA-IS 后缀数组与 Kasai LCP；字符集已压缩到 [0,up] 时为 O(n)。
用法：build(string) 直接处理字节串；整数序列先压缩后调用 build(a, up)。
约定：返回 sa，所有下标为 int；通用 build 会排序压缩，复杂度为 O(n log n)。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
struct SA {

    static vector<int> sais(const vector<int>& s, int up) {
        int n = s.size();
        assert(up >= 0);
        for (int x : s) assert(0 <= x && x <= up);
        if (!n) return {};
        if (n == 1) return {0};
        if (n == 2) return s[0] < s[1] ? vector<int>{0, 1} : vector<int>{1, 0};

        vector<uint8_t> tp(n);
        for (int i = n - 2; i >= 0; --i) {
            tp[i] = s[i] == s[i + 1] ? tp[i + 1] : s[i] < s[i + 1];
        }
        vector<int> sl(up + 2), ss(up + 2), sa(n);
        for (int i = 0; i < n; ++i) {
            if (tp[i])
                ++sl[s[i] + 1];
            else
                ++ss[s[i]];
        }
        for (int i = 0; i <= up; ++i) {
            ss[i] += sl[i];
            sl[i + 1] += ss[i];
        }

        auto ind = [&](const vector<int>& lms) {
            fill(sa.begin(), sa.end(), -1);
            vector<int> b = ss;
            for (int x : lms)
                if (x < n) sa[b[s[x]]++] = x;
            b = sl;
            sa[b[s[n - 1]]++] = n - 1;
            for (int i = 0; i < n; ++i) {
                int x = sa[i];
                if (x > 0 && !tp[x - 1]) sa[b[s[x - 1]]++] = x - 1;
            }
            b = sl;
            for (int i = n - 1; i >= 0; --i) {
                int x = sa[i];
                if (x > 0 && tp[x - 1]) sa[--b[s[x - 1] + 1]] = x - 1;
            }
        };

        vector<int> id(n + 1, -1), lms;
        int m = 0;
        for (int i = 1; i < n; ++i)
            if (!tp[i - 1] && tp[i]) id[i] = m++, lms.push_back(i);
        ind(lms);
        if (!m) return sa;

        vector<int> ord;
        ord.reserve(m);
        for (int x : sa)
            if (id[x] != -1) ord.push_back(x);
        vector<int> rec(m);
        int rup = 0;
        rec[id[ord[0]]] = 0;
        for (int i = 1; i < m; ++i) {
            int x = ord[i - 1], y = ord[i];
            int rx = id[x] + 1 < m ? lms[id[x] + 1] : n;
            int ry = id[y] + 1 < m ? lms[id[y] + 1] : n;
            bool sam = rx - x == ry - y;
            for (; sam && x < rx; ++x, ++y)
                if (s[x] != s[y]) break;
            if (x == n || s[x] != s[y]) sam = false;
            if (!sam) ++rup;
            rec[id[ord[i]]] = rup;
        }
        auto rsa = sais(rec, rup);
        for (int i = 0; i < m; ++i) ord[i] = lms[rsa[i]];
        ind(ord);
        return sa;
    }

    static vector<int> build(const string& s) {
        vector<int> a(s.size());
        for (int i = 0; i < s.size(); ++i) a[i] = static_cast<unsigned char>(s[i]);
        return sais(a, 255);
    }

    static vector<int> build(const vector<int>& s, int up) {
        return sais(s, up);
    }

    template <class S> static vector<int> build(const S& s) {
        int n = s.size();
        vector<int> a(n), p(n);
        iota(p.begin(), p.end(), 0);
        sort(p.begin(), p.end(), [&](int x, int y) { return s[x] < s[y]; });
        int up = 0;
        for (int i = 0; i < n; ++i) {
            if (i && s[p[i - 1]] != s[p[i]]) ++up;
            a[p[i]] = up;
        }
        return sais(a, up);
    }

    template <class S> static vector<int> askLCP(const S& s, const vector<int>& sa) {
        int n = s.size();
        assert(sa.size() == n);
        vector<int> rk(n), a(n ? n - 1 : 0);
        for (int i = 0; i < n; ++i) {
            assert(0 <= sa[i] && sa[i] < n);
            rk[sa[i]] = i;
        }
        for (int i = 0, h = 0; i < n; ++i) {
            if (!rk[i]) continue;
            int j = sa[rk[i] - 1];
            while (i + h < n && j + h < n && s[i + h] == s[j + h]) ++h;
            a[rk[i] - 1] = h;
            if (h) --h;
        }
        return a;
    }
};
