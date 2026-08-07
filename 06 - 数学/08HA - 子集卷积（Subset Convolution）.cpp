/*
用途：c[S] = sum(A subset S) a[A] b[S-A]。
前提：长度为 2^k，Z 支持加减乘；复杂度 O(k^2 2^k)。
*/
namespace SubConv {
template <class Z> void zetaSub(vector<Z>& a) {
    int n = a.size(), k = __builtin_ctz((unsigned)n);
    assert(n && !(n & (n - 1)));
    for (int i = 0; i < k; ++i)
        for (int s = 0; s < n; ++s)
            if (s >> i & 1) a[s] += a[s ^ (1 << i)];
}

template <class Z> void mobSub(vector<Z>& a) {
    int n = a.size(), k = __builtin_ctz((unsigned)n);
    assert(n && !(n & (n - 1)));
    for (int i = 0; i < k; ++i)
        for (int s = 0; s < n; ++s)
            if (s >> i & 1) a[s] -= a[s ^ (1 << i)];
}

template <class Z> vector<Z> mul(const vector<Z>& a, const vector<Z>& b) {
    int n = a.size(), k = __builtin_ctz((unsigned)n);
    assert(n && !(n & (n - 1)) && b.size() == n);
    vector<vector<Z>> x(k + 1, vector<Z>(n)), y(k + 1, vector<Z>(n)), z(k + 1, vector<Z>(n));
    for (int s = 0; s < n; ++s) {
        int d = __builtin_popcount((unsigned)s);
        x[d][s] = a[s];
        y[d][s] = b[s];
    }
    for (int i = 0; i <= k; ++i) {
        zetaSub(x[i]);
        zetaSub(y[i]);
    }
    for (int i = 0; i <= k; ++i) {
        for (int j = 0; j <= i; ++j)
            for (int s = 0; s < n; ++s) z[i][s] += x[j][s] * y[i - j][s];
        mobSub(z[i]);
    }
    vector<Z> r(n);
    for (int s = 0; s < n; ++s) r[s] = z[__builtin_popcount((unsigned)s)][s];
    return r;
}
}  // namespace SubConv
