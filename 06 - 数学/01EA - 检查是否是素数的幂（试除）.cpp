/*
用途：判断 n 是否可写成 p^k（p 为质数，k >= 1）。
复杂度：O(sqrt n)，无预处理；适合单次或少量查询。已筛范围内的大量查询使用 01EB。
*/

inline bool ask(int n) {
    if (n <= 1) return false;
    for (int p = 2; p <= n / p; ++p) {
        if (n % p != 0) continue;
        do n /= p; while (n % p == 0);
        return n == 1;
    }
    return true;  // 没有真因子，n 本身为质数。
}
