/*
用途：完全背包；每种物品可取任意多次。
约定：容量下标使用 int；返回 f[j] 表示容量不超过 j 的最大价值。
复杂度：O(nm) 时间、O(m) 空间。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <class T>
vector<T> packAll(int m, const vector<pair<int, T>>& a) {
    assert(m >= 0);
    vector<T> f(m + 1);
    for (auto [w, v] : a) {
        assert(w > 0);
        for (int j = w; j <= m; ++j) f[j] = max(f[j], f[j - w] + v);
    }
    return f;
}
