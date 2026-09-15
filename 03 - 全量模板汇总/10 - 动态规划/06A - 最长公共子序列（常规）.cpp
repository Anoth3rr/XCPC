/*
用途：任意可比较序列的 LCS；返回一组最长公共子序列。
复杂度：O(nm) 时间、O(nm) 空间。只求字符串长度且 m 很大时使用 06B。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <class Seq>
vector<typename Seq::value_type> lcs(const Seq& a, const Seq& b) {
    int n = a.size(), m = b.size();
    vector f(n + 1, vector<int>(m + 1));
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (a[i - 1] == b[j - 1]) f[i][j] = f[i - 1][j - 1] + 1;
            else f[i][j] = max(f[i - 1][j], f[i][j - 1]);
        }
    }
    vector<typename Seq::value_type> r;
    for (int i = n, j = m; i && j;) {
        if (a[i - 1] == b[j - 1]) r.push_back(a[--i]), --j;
        else if (f[i - 1][j] >= f[i][j - 1]) --i;
        else --j;
    }
    reverse(r.begin(), r.end());
    return r;
}
