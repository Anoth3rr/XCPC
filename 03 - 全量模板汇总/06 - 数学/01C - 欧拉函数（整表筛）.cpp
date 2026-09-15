/*
用途：预处理 phi(1..n)。
复杂度：O(n log log n)；若同时还需 minPrime / mu，使用 01 - 素数（线性筛）的 LinearSieve::phi（O(n)）。
*/
inline vector<int> phiAll(int n) {
    assert(n >= 0);
    vector<int> phi(n + 1);
    iota(phi.begin(), phi.end(), 0);
    if (n >= 1) phi[1] = 1;
    for (int p = 2; p <= n; ++p) {
        if (phi[p] != p) continue;
        for (int x = p; x <= n; x += p) phi[x] = phi[x] / p * (p - 1);
    }
    return phi;
}
