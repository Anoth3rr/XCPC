/*
用途：判断 n 是否可写成 p^k（p 为质数，k >= 1）。
前提：使用 01 - 素数（线性筛）得到 minPrime，且 n 在筛范围内。
复杂度：O(log n)；大量、范围已知的查询优于 01EA。
*/
inline bool ask(int n, const vector<int>& lp) {
    if (n <= 1 || n >= lp.size()) return false;
    int p = lp[n];
    if (p <= 1) return false;
    do n /= p; while (n % p == 0);
    return n == 1;
}
