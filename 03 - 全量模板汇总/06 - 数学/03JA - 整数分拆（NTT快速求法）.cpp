/*
用途：用形式幂级数快速求整数分拆 p(0..n)，即不计顺序的正整数分解。
依赖：02A - 自动取模（MInt）、08B 或 08C - 多项式（Poly, Z）。
公式：prod_{i>=1}(1-x^i)^(-1) = exp(sum_{k>=1} sigma(k)x^k/k)。
复杂度：O(n log n)（sigma 用约数筛，exp 由 Poly 完成）；小 n 使用 03J 常数更小。
限制：Z 的模数须为 NTT 友好质数，且 n < mod。
*/
template <class Z, int G = 3> vector<Z> partFast(int n) {
    assert(0 <= n && n < Z::askMod());
    vector<Z> f(n + 1);
    for (int d = 1; d <= n; ++d)
        for (int k = d; k <= n; k += d) f[k] += Z(d);
    for (int k = 1; k <= n; ++k) f[k] /= Z(k);
    return Poly<Z, G>::exp(f, n + 1);
}
