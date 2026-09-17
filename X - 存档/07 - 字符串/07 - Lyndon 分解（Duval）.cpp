/*
用途：Lyndon 分解，返回每个 Lyndon 因子的半开区间。
约定：返回的因子字典序单调不增；元素类型只需可比较。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <class S> vector<array<int, 2>> duval(const S& s) {
    int n = s.size();
    vector<array<int, 2>> a;
    for (int i = 0; i < n;) {
        int j = i + 1, k = i;
        while (j < n && s[k] <= s[j]) {
            if (s[k] < s[j])
                k = i;
            else
                ++k;
            ++j;
        }
        int w = j - k;
        while (i <= k) {
            a.push_back({i, i + w});
            i += w;
        }
    }
    return a;
}
