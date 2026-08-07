/*
用途：预处理 Catalan(0..n)。
前提：Z 的模数为质数，n < mod。
*/
template <class Z> vector<Z> catalan(int n) {
    assert(n >= 0 && n < Z::askMod());
    vector<Z> a(n + 1);
    a[0] = 1;
    for (int i = 1; i <= n; ++i) a[i] = a[i - 1] * Z(4 * i - 2) / Z(i + 1);
    return a;
}
