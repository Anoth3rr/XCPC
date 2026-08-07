/*
用途：已知 f(0..n-1)，在任意整数 x 处求次数小于 n 的多项式值。
前提：Z 为质数模，n < mod；复杂度 O(n)。
*/

template <class Z> Z lagr(const vector<Z>& a, int x) {
    int n = a.size();
    assert(n && n < Z::askMod());
    if (0 <= x && x < n) return a[x];
    Z p = x;
    vector<Z> l(n + 1, Z(1)), r(n + 1, Z(1)), f(n, Z(1)), g(n, Z(1));
    for (int i = 0; i < n; ++i) l[i + 1] = l[i] * (p - Z(i));
    for (int i = n - 1; i >= 0; --i) r[i] = r[i + 1] * (p - Z(i));
    for (int i = 1; i < n; ++i) f[i] = f[i - 1] * i;
    g[n - 1] = f[n - 1].inv();
    for (int i = n - 1; i; --i) g[i - 1] = g[i] * i;
    Z ans = 0;
    for (int i = 0; i < n; ++i) {
        Z t = a[i] * l[i] * r[i + 1] * g[i] * g[n - 1 - i];
        if ((n - 1 - i) & 1) t = -t;
        ans += t;
    }
    return ans;
}
