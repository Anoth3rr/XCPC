/*
用途：有依赖背包；选择节点前必须选择父节点，多个 parent=0 的根可独立选择。
输入：w/v/parent 均为 1-indexed 等长数组，容量 m；返回每个“恰好容量”的最优值。
复杂度：O(nm^2)，适合树形依赖与中小容量；不可达状态为 NEG。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <class T>
vector<T> packDep(int m, const vector<int>& w, const vector<T>& v,
                            const vector<int>& fa) {
    static_assert(is_signed_v<T>);
    assert(m >= 0 && w.size() == v.size() && v.size() == fa.size());
    int n = w.size() - 1;
    vector<vector<int>> g(n + 1);
    for (int i = 1; i <= n; ++i) {
        assert(0 <= fa[i] && fa[i] <= n && fa[i] != i && w[i] >= 0);
        g[fa[i]].push_back(i);
    }
    const T NEG = numeric_limits<T>::lowest() / 4;
    int vis = 0;
    auto dfs = [&](auto&& go, int u) -> vector<T> {
        vis += u != 0;
        vector<T> f(m + 1, NEG);
        int s = u ? w[u] : 0;
        if (s <= m) f[s] = u ? v[u] : T{};
        for (int x : g[u]) {
            auto d = go(go, x);
            vector<T> h = f;
            for (int i = 0; i <= m; ++i) if (f[i] != NEG)
                for (int j = 0; i + j <= m; ++j) if (d[j] != NEG)
                    h[i + j] = max(h[i + j], f[i] + d[j]);
            f.swap(h);
        }
        return f;
    };
    auto f = dfs(dfs, 0);
    assert(vis == n);  // 否则 parent 中存在不连向虚根的环。
    return f;
}
