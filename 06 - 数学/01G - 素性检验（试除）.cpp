/*
用途：无预处理地判断单个小整数是否为质数。
复杂度：O(sqrt n)。与 01F 是同一问题的不同环境：小数、少量查询用本模板；int 大数用 Miller-Rabin。
*/

inline bool ask(int x) {
    if (x < 2) return false;
    for (int d = 2; d <= x / d; ++d) {
        if (x % d == 0) return false;
    }
    return true;
}
