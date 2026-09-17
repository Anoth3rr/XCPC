/*
用途：已知 f(0),...,f(n)，求 f(m),...,f(m+n)。
依赖：02A - 自动取模（MInt）、08B - 多项式（Poly, Z）。
接口：shift<Z,3>(f,m)，f 的下标即横坐标。
前提：m == 0，或 n < m 且 m+n < mod；横坐标分母必须在模意义下可逆。
复杂度：O(n log n)，模数须为 NTT 友好质数。
*/

template <class Z, int G = 3> vector<Z> shift(vector<Z> f, int m) {
    int n = f.size() - 1;
    assert(n >= 0);
    if (!m) return f;
    assert(n < m && m + n < Z::askMod());
    vector<Z> fac(n + 1, Z(1)), ifac(n + 1, Z(1));
    for (int i = 1; i <= n; ++i) fac[i] = fac[i - 1] * Z(i);
    ifac[n] = fac[n].inv();
    for (int i = n; i > 0; --i) ifac[i - 1] = ifac[i] * Z(i);

    vector<Z> a(n + 1), b(n * 2 + 1);
    for (int i = 0; i <= n; ++i) {
        a[i] = f[i] * ifac[i] * ifac[n - i];
        if ((n - i) & 1) a[i] = -a[i];
    }
    for (int i = 0; i <= n * 2; ++i) b[i] = Z(m - n + i).inv();
    a = Poly<Z, G>::mul(a, b);

    Z cur = 1;
    for (int i = m - n; i <= m; ++i) cur *= Z(i);
    for (int i = 0; i <= n; ++i) {
        f[i] = a[n + i] * cur;
        cur *= Z(m + i + 1);
        cur /= Z(m - n + i);
    }
    return f;
}
