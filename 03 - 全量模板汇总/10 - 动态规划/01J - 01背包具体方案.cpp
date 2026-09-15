/*
用途：01 背包求最大价值并恢复一组方案；能保持最优时优先选择编号更小的物品。
输入：w/v 为 0-indexed 等长数组，容量 m；返回 {最大价值, 选中下标}。
复杂度：O(nm) 时间与空间。只求最优值时使用 01A 的一维版本。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <class T>
pair<T, vector<int>> packSol(int m, const vector<int>& w, const vector<T>& v) {
    assert(m >= 0 && w.size() == v.size());
    int n = w.size();
    vector<vector<T>> f(n + 1, vector<T>(m + 1));
    for (int i = n; i-- > 0;) {
        assert(w[i] > 0);
        for (int j = 0; j <= m; ++j) {
            f[i][j] = f[i + 1][j];
            if (w[i] <= j) f[i][j] = max(f[i][j], f[i + 1][j - w[i]] + v[i]);
        }
    }
    vector<int> a;
    int c = m;
    for (int i = 0; i < n; ++i) if (w[i] <= c && f[i][c] == f[i + 1][c - w[i]] + v[i])
        a.push_back(i), c -= w[i];
    return {f[0][m], a};
}
