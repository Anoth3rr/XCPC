/*
用途：区间 DP：f[l][r]=min(f[l][k]+f[k+1][r])+cost(l,r)。
前提：cost 满足四边形不等式且区间包含单调，因而 opt[l][r-1]<=opt[l][r]<=opt[l+1][r]。
下标：0..n-1。复杂度 O(n^2) 时间、O(n^2) 空间。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <class T, class C>
pair<vector<vector<T>>, vector<vector<int>>> knuthDP(int n, C cst) {
    assert(n >= 0);
    vector f(n, vector<T>(n));
    vector p(n, vector<int>(n));
    for (int i = 0; i < n; ++i) p[i][i] = i;
    for (int d = 1; d < n; ++d) {
        for (int l = 0, r = d; r < n; ++l, ++r) {
            int a = p[l][r - 1];
            int b = min<int>(r - 1, p[l + 1][r]);
            f[l][r] = numeric_limits<T>::max() / 4;
            for (int k = a; k <= b; ++k) {
                T x = f[l][k] + f[k + 1][r] + cst(l, r);
                if (x < f[l][r]) f[l][r] = x, p[l][r] = k;
            }
        }
    }
    return {move(f), move(p)};
}
