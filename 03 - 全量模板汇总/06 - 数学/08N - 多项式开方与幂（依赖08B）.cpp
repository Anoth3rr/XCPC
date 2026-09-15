/*
用途：形式幂级数平方根与非负整数幂。
依赖：02A - 自动取模（MInt）、02E - 模平方根、08B - 多项式（Poly, Z）。
接口：psqrt<Z,3>(f,n) 返回截断到 n 项的平方根；ppow<Z,3>(f,e,n) 返回 f^e 的前 n 项。
前提：psqrt 的首个非零项次数为偶数且首项为二次剩余；模数须为 NTT 友好奇质数。
复杂度：O(n log n)。psqrt 无解返回 nullopt。
*/

template <class Z, int G = 3> optional<vector<Z>> psqrt(vector<Z> f, int n) {
    assert(n >= 0);
    f.resize(n);
    int p = 0;
    while (p < n && f[p] == Z(0)) ++p;
    if (p == n) return vector<Z>(n);
    if (p & 1) return nullopt;

    int sh = p >> 1, m = n - p;
    vector<Z> a(f.begin() + p, f.end());
    auto rt = sqrt(a[0]);
    if (!rt) return nullopt;
    vector<Z> b{*rt};
    Z iv = Z(2).inv();
    for (int k = 2; k < m << 1; k <<= 1) {
        int len = min(k, m);
        vector<Z> c(a.begin(), a.begin() + min<int>(a.size(), len));
        c.resize(len);
        c = Poly<Z, G>::mul(c, Poly<Z, G>::inv(b, len));
        c.resize(len);
        b.resize(len);
        for (int i = 0; i < len; ++i) b[i] = (b[i] + c[i]) * iv;
    }
    vector<Z> ans(n);
    for (int i = 0; i < b.size() && i + sh < n; ++i) ans[i + sh] = b[i];
    return ans;
}

template <class Z, int G = 3> vector<Z> ppow(vector<Z> f, int e, int n) {
    assert(e >= 0 && n >= 0);
    if (!n) return {};
    if (!e) {
        vector<Z> ans(n);
        ans[0] = Z(1);
        return ans;
    }
    int p = 0;
    while (p < f.size() && f[p] == Z(0)) ++p;
    if (p == f.size() || (p && e > (n - 1) / p)) return vector<Z>(n);

    int sh = p * e, m = n - sh;
    Z c = f[p];
    vector<Z> a(f.begin() + p, f.end());
    a.resize(m);
    for (Z& x : a) x /= c;
    a = Poly<Z, G>::ln(a, m);
    for (Z& x : a) x *= Z(e);
    a = Poly<Z, G>::exp(a, m);
    c = c.pow(e);
    vector<Z> ans(n);
    for (int i = 0; i < m; ++i) ans[i + sh] = a[i] * c;
    return ans;
}
