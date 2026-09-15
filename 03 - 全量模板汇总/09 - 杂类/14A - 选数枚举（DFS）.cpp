/* 用途：枚举 n 个数中恰选 k 个；回调参数为当前元素下标与元素和。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <class T, class F> void choose(const vector<T> &a, int k, F f) {
    vector<int> q;
    auto dfs = [&](auto go, int p, T s) -> void {
        if (q.size() == k) {
            f(q, s);
            return;
        }
        int d = k - q.size();
        for (int i = p; i + d <= a.size(); ++i) {
            q.push_back(i);
            go(go, i + 1, s + a[i]);
            q.pop_back();
        }
    };
    dfs(dfs, 0, T{});
}
