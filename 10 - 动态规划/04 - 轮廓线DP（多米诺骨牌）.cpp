/*
用途：用 1x2 多米诺骨牌铺满 n*m 网格的方案数。
T 可取 int、高精整数或 MInt（自动取模）。宽度取 min(n,m)，通常不超过 15。
复杂度：逐格转移 O(n * m * 2^m)。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <class T>
T domino(int n, int m) {
    assert(n >= 0 && m >= 0);
    if (n < m) swap(n, m);
    if (!m) return T(1);
    assert(m < 31);
    int z = 1LL << m;
    vector<T> f(z), g(z);
    f[0] = T(1);
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < m; ++c) {
            fill(g.begin(), g.end(), T{});
            for (int s = 0; s < z; ++s) {
                if (s & 1) g[s >> 1] += f[s];
                else {
                    if (r + 1 < n) g[(s >> 1) | (1LL << (m - 1))] += f[s];
                    if (c + 1 < m && !(s & 2)) g[(s >> 1) | 1] += f[s];
                }
            }
            f.swap(g);
        }
    }
    return f[0];
}
