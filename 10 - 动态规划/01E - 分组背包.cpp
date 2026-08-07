/*
用途：分组背包；每组至多选择一件物品。
groups[g] 中每个元素为 {体积, 价值}。
复杂度：O(m * 物品总数)。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <class T>
vector<T> packGrp(int m, const vector<vector<pair<int, T>>>& a) {
    assert(m >= 0);
    vector<T> f(m + 1);
    for (const auto& g : a) {
        vector<T> h = f;
        for (auto [w, v] : g) {
            assert(w > 0);
            for (int j = w; j <= m; ++j) h[j] = max(h[j], f[j - w] + v);
        }
        f.swap(h);
    }
    return f;
}
