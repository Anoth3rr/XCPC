/*
用途：使用最小质因子表分解小范围整数。
前提：先构造 LinearSieve sieve(maxValue)，且 1 <= n <= sieve.limit。
复杂度：O(log n)，适合大量、范围已知的查询；大整数请使用 01FA - Pollard-Rho。
*/
inline vector<pair<int, int>> factor(int n, const vector<int>& lp) {
    assert(1 <= n && n < lp.size());
    vector<pair<int, int>> res;
    while (n > 1) {
        int p = lp[n], e = 0;
        do {
            n /= p;
            ++e;
        } while (n % p == 0);
        res.push_back({p, e});
    }
    return res;
}
