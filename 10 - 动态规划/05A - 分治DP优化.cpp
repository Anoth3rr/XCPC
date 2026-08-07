/*
用途：计算 g[j] = min_{0<=k<j}(f[k] + cost(k+1,j))。
前提：最优决策点随 j 单调；单调性不成立时结果错误。
调用：divide(1,n,0,n-1,f,g,p,cst,inf)。
复杂度：每一层 DP O(n log n) 次转移计算，常见进一步可证为 O(n log n)。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <class T, class C>
void divide(int l, int r, int ql, int qr,
              const vector<T>& f, vector<T>& g, vector<int>& p,
              C cst, T inf) {
    if (l > r) return;
    int m = (l + r) >> 1;
    pair<T, int> z{inf, -1};
    int up = min(qr, m - 1);
    for (int k = ql; k <= up; ++k) {
        if (f[k] == inf) continue;
        z = min(z, pair<T, int>{f[k] + cst(k + 1, m), k});
    }
    g[m] = z.first;
    p[m] = z.second;
    if (l == r) return;
    int q = z.second == -1 ? ql : z.second;
    divide(l, m - 1, ql, q, f, g, p, cst, inf);
    divide(m + 1, r, q, qr, f, g, p, cst, inf);
}
