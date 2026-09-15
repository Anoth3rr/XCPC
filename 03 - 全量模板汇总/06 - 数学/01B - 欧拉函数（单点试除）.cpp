/*
用途：计算单个 phi(n)。
复杂度：O(sqrt n)，无须预处理；需要整张 phi 表时使用 01C，需要 minPrime / mu 等更多信息时使用 01 - 素数（线性筛）。
*/

inline int phi(int n) {
    assert(n >= 1);
    int res = n;
    for (int p = 2; p <= n / p; ++p) {
        if (n % p != 0) continue;
        while (n % p == 0) n /= p;
        res = res / p * (p - 1);
    }
    if (n > 1) res = res / n * (n - 1);
    return res;
}
