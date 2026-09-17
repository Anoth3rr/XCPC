/*
用途：Held-Karp 状压 DP，求从 s 出发、恰好访问每个点一次并回到 s 的最短 Hamilton 回路。
w[u][v] 为边权；不可达写 INF。n 通常不超过 20。
复杂度：O(2^n n^2) 时间、O(2^n n) 空间。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <class T>
optional<T> tsp(const vector<vector<T>>& w, int s = 0,
                T inf = numeric_limits<T>::max() / 4) {
    int n = w.size();
    assert(n > 0 && n <= 22 && 0 <= s && s < n);
    for (const auto& a : w) assert(a.size() == n);
    int z = 1LL << n;
    vector f(z, vector<T>(n, inf));
    f[1LL << s][s] = T{};
    for (int m = 0; m < z; ++m) {
        if (!(m >> s & 1)) continue;
        for (int u = 0; u < n; ++u) {
            if (!(m >> u & 1) || f[m][u] == inf) continue;
            for (int v = 0; v < n; ++v) {
                if (m >> v & 1 || w[u][v] == inf) continue;
                int t = m | (1LL << v);
                f[t][v] = min(f[t][v], f[m][u] + w[u][v]);
            }
        }
    }
    T r = inf;
    for (int u = 0; u < n; ++u)
        if (u != s && f[z - 1][u] != inf && w[u][s] != inf) r = min(r, f[z - 1][u] + w[u][s]);
    if (n == 1) r = T{};
    return r == inf ? nullopt : optional<T>(r);
}
