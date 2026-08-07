/*
用途：按输入顺序，把物品放入最靠前且剩余容量足够的箱子（First Fit）。
说明：这是指定在线策略，不保证最少箱数；O(n log n)。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <class T> int binPack(const vector<T> &a, T c) {
    int n = a.size();
    vector<T> t(n * 4, c);
    auto sub = [&](auto go, int p, int l, int r, T x) -> int {
        if (l + 1 == r) {
            t[p] -= x;
            return l;
        }
        int m = (l + r) >> 1, q;
        if (t[p << 1] >= x) q = go(go, p << 1, l, m, x);
        else q = go(go, p << 1 | 1, m, r, x);
        t[p] = max(t[p << 1], t[p << 1 | 1]);
        return q;
    };
    int z = 0;
    for (T x : a) {
        if (x > c) return -1;
        z = max(z, sub(sub, 1, 0, n, x) + 1);
    }
    return z;
}
