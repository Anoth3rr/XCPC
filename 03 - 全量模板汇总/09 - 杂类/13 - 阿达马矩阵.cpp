/*
用途：Sylvester 构造 2^k 阶阿达马矩阵；元素为 +1/-1，任意两行正交。
复杂度：O(4^k)，输出矩阵本身即需要该规模。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
vector<vector<int>> hadam(int k) {
    int n = 1LL << k;
    vector a(n, vector<int>(n, 1));
    for (int s = 1; s < n; s <<= 1)
        for (int i = 0; i < s; ++i)
            for (int j = 0; j < s; ++j) {
                a[i][j + s] = a[i][j];
                a[i + s][j] = a[i][j];
                a[i + s][j + s] = -a[i][j];
            }
    return a;
}
