/*
用途：预处理错排数 D(0..n) 与 Bell 数 B(0..n)。
仅要求 Z 支持加减乘。
*/
template <class Z> vector<Z> derange(int n) {
    assert(n >= 0);
    vector<Z> a(n + 1);
    a[0] = 1;
    for (int i = 2; i <= n; ++i) a[i] = Z(i - 1) * (a[i - 1] + a[i - 2]);
    return a;
}

template <class Z> vector<Z> bell(int n) {
    assert(n >= 0);
    vector<vector<Z>> a(n + 1);
    vector<Z> b(n + 1);
    a[0] = {Z(1)};
    b[0] = 1;
    for (int i = 1; i <= n; ++i) {
        a[i].assign(i + 1, Z(0));
        a[i][0] = a[i - 1][i - 1];
        for (int j = 1; j <= i; ++j) a[i][j] = a[i][j - 1] + a[i - 1][j - 1];
        b[i] = a[i][0];
    }
    return b;
}
