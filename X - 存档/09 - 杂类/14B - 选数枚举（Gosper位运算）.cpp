/* 用途：n<=63 时枚举所有恰含 k 个 1 的掩码；回调参数为 u64 掩码。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/

using u64 = uint64_t;

template <class F> void choose(int n, int k, F f) {
    if (k < 0 || k > n || n > 63) return;
    if (!k) {
        f(u64(0));
        return;
    }
    u64 x = (u64(1) << k) - 1, u = u64(1) << n;
    while (x < u) {
        f(x);
        u64 a = x & -x, b = x + a;
        x = b | ((x ^ b) >> 2) / a;
    }
}
