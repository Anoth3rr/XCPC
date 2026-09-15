/*
用途：循环串的最小或最大字典序表示起点。
用法：booth(s) 求最小表示法；booth<false>(s) 求最大表示法。
约定：返回 0-indexed 起点；空串返回 0；元素类型只需可比较。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <bool Min = true, class S> int booth(const S& s) {
    int n = s.size();
    if (!n) return 0;
    int i = 0, j = 1, k = 0;
    while (i < n && j < n && k < n) {
        auto x = s[(i + k) % n], y = s[(j + k) % n];
        if (x == y) {
            ++k;
            continue;
        }
        if ((x > y) == Min)
            i += k + 1;
        else
            j += k + 1;
        if (i == j) ++j;
        k = 0;
    }
    return min(i, j);
}
