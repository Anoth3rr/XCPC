/*
用途：在下标 1..N 上计算 Dirichlet convolution：
  (f*g)(n)=sum_{d|n} f(d)g(n/d)，并求 f 的 Dirichlet 逆。
前提：数组下标 0 被忽略；求逆要求 f(1) 可逆。复杂度 O(N log N)。
关系：与 01DA 的约数 zeta / Möbius 变换配套使用。
*/
namespace Dirichlet {
template <class T> vector<T> mul(const vector<T>& l, const vector<T>& r) {
    int n = min(l.size(), r.size()) - 1;
    assert(n >= 0);
    vector<T> ans(n + 1, T(0));
    for (int d = 1; d <= n; ++d) {
        for (int k = d; k <= n; k += d) {
            ans[k] += l[d] * r[k / d];
        }
    }
    return ans;
}

template <class Z> vector<Z> inverse(const vector<Z>& f) {
    int n = f.size() - 1;
    assert(n >= 1 && f[1] != Z(0));
    vector<Z> ans(n + 1, Z(0));
    ans[1] = f[1].inv();
    vector<Z> sum(n + 1, Z(0));
    for (int d = 2; d <= n; ++d) sum[d] += f[d] * ans[1];
    for (int val = 2; val <= n; ++val) {
        ans[val] = -sum[val] * ans[1];
        for (int d = 2; val <= n / d; ++d) {
            sum[val * d] += f[d] * ans[val];
        }
    }
    return ans;
}
}  // namespace Dirichlet
