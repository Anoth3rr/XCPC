/*
用途：统计 [0,n] 中十进制表示被有限状态自动机接受的整数个数。
go(state, digit) 返回下一状态；若需要区分前导零，把 started 一并编码进状态。
good(state) 判断读完固定宽度（含前导零）后的状态是否接受。
T 可取 int、高精整数或 MInt，后者会自动取模。
复杂度：O(位数 * 状态数 * 10)。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <class T, class Go, class G>
T digitDP(int n, int sc, int st, Go go, G ok) {
    assert(sc > 0 && 0 <= st && st < sc);
    vector<int> d;
    do {
        d.push_back(n % 10);
        n /= 10;
    } while (n);
    reverse(d.begin(), d.end());

    int m = d.size();
    vector f(m + 1, vector<T>(sc));
    vector vis(m + 1, vector<char>(sc));
    auto dfs = [&](auto&& rec, int p, int s, bool lim) -> T {
        if (p == m) return T(ok(s));
        if (!lim && vis[p][s]) return f[p][s];
        int up = lim ? d[p] : 9;
        T r{};
        for (int x = 0; x <= up; ++x) {
            int t = go(s, x);
            assert(0 <= t && t < sc);
            r += rec(rec, p + 1, t, lim && x == up);
        }
        if (!lim) vis[p][s] = 1, f[p][s] = r;
        return r;
    };
    return dfs(dfs, 0, st, true);
}

template <class T, class Go, class G>
T digitDP(int l, int r, int sc, int st, Go go, G ok) {
    assert(l <= r);
    T x = digitDP<T>(r, sc, st, go, ok);
    return l ? x - digitDP<T>(l - 1, sc, st, go, ok) : x;
}
