/*
用途：用形式幂级数快速求 Bernoulli 数 B_0..B_n，约定 B_1=-1/2。
依赖：02A - 自动取模（MInt）、08B 或 08C - 多项式（Poly, Z）。
公式：x/(exp(x)-1) = sum B_k x^k/k!。
复杂度：O(n log n)；普通 14 的 O(n^2) 版本适合小阶数并直接支持幂和查询。
限制：Z 的模数须为 NTT 友好质数，且 n + 1 < mod。
*/
template <class Z, int G = 3> vector<Z> bernFast(int n) {
    assert(0 <= n && n + 1 < Z::askMod());
    vector<Z> fac(n + 2), iv(n + 2), h(n + 1);
    fac[0] = 1;
    for (int i = 1; i <= n + 1; ++i) fac[i] = fac[i - 1] * Z(i);
    iv[n + 1] = fac[n + 1].inv();
    for (int i = n + 1; i; --i) iv[i - 1] = iv[i] * Z(i);
    for (int i = 0; i <= n; ++i) h[i] = iv[i + 1];
    vector<Z> a = Poly<Z, G>::inv(h, n + 1);
    for (int i = 0; i <= n; ++i) a[i] *= fac[i];
    return a;
}
