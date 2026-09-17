/*
用途：用指数型生成函数快速求 Bell(0..n)。
依赖：02A - 自动取模（MInt）、08B 或 08C - 多项式（Poly, Z）。
公式：sum Bell(n)x^n/n! = exp(exp(x)-1)。
复杂度：O(n log n)（由 Poly::exp 完成），比 03F 的 O(n^2) 适合大 n。
限制：Z 的模数须为 NTT 友好质数，且 n < mod；小 n 使用 03F 常数更小。
*/
template <class Z, int G = 3> vector<Z> bellFast(int n) {
    assert(0 <= n && n < Z::askMod());
    vector<Z> iv(n + 1), f(n + 1), fac(n + 1);
    fac[0] = 1;
    for (int i = 1; i <= n; ++i) {
        fac[i] = fac[i - 1] * Z(i);
    }
    iv[n] = fac[n].inv();
    for (int i = n; i; --i) iv[i - 1] = iv[i] * Z(i);
    f[0] = 0;
    for (int i = 1; i <= n; ++i) f[i] = iv[i];
    vector<Z> a = Poly<Z, G>::exp(f, n + 1);
    for (int i = 0; i <= n; ++i) a[i] *= fac[i];
    return a;
}
