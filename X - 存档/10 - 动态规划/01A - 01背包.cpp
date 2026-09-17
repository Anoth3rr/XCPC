/*
用途：01 背包；每件物品至多取一次，求容量不超过 m 的最大价值。
约定：容量下标使用 int；T 是价值类型。返回 f[j] 表示容量上限为 j 的答案。
复杂度：O(nm) 时间、O(m) 空间。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <class T>
vector<T> pack01(int m, const vector<pair<int, T>>& a) {
    assert(m >= 0);
    vector<T> f(m + 1);
    for (auto [w, v] : a) {
        assert(w > 0);
        for (int j = m; j >= w; --j) f[j] = max(f[j], f[j - w] + v);
    }
    return f;
}

// 恰好装满。不可达状态为 neg，调用者应保证 neg + v 不溢出。
template <class T>
vector<T> packEq(int m, const vector<pair<int, T>>& a,
                      T neg = numeric_limits<T>::lowest() / 4) {
    assert(m >= 0);
    vector<T> f(m + 1, neg);
    f[0] = T{};
    for (auto [w, v] : a) {
        assert(w > 0);
        for (int j = m; j >= w; --j) {
            if (f[j - w] != neg) f[j] = max(f[j], f[j - w] + v);
        }
    }
    return f;
}
