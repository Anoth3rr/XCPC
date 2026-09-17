/*
用途：01 背包恰好装满时，同时求最大价值和达到最大值的方案数。
Z 可取 int（精确计数）或 MInt（自动取模）。每件物品按下标区分。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <class T, class Z> struct PackCount {
    T val;
    Z cnt;
};

template <class T, class Z>
vector<PackCount<T, Z>> packCnt(int m, const vector<pair<int, T>>& a,
                                         T neg = numeric_limits<T>::lowest() / 4) {
    vector<PackCount<T, Z>> f(m + 1, {neg, Z{}});
    f[0] = {T{}, Z(1)};
    for (auto [w, v] : a) {
        assert(w > 0);
        for (int j = m; j >= w; --j) {
            if (f[j - w].val == neg) continue;
            T x = f[j - w].val + v;
            if (x > f[j].val) f[j] = {x, f[j - w].cnt};
            else if (x == f[j].val) f[j].cnt += f[j - w].cnt;
        }
    }
    return f;
}
