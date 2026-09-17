/*
用途：计算 a^b mod m，其中 b 或 a 可用非负十进制字符串给出。
依赖：02 - 快速幂（ModArith64）。
接口：askMod(s,m) 求十进制串模 m；solve(a,b,m) 或 solve(aStr,b,m)。
复杂度：O(|a| + |b| log 10)。不需要 gcd(a,m)=1，也不依赖欧拉降幂。
*/

namespace BigPow {
inline int askMod(const string& s, int md) {
    assert(md > 0 && !s.empty());
    int ans = 0;
    for (char c : s) {
        assert('0' <= c && c <= '9');
        ans = (ModArith64::mul(ans, 10, md) + c - '0') % md;
    }
    return ans;
}

inline int solve(int a, const string& b, int md) {
    assert(md > 0 && !b.empty());
    a %= md;
    if (a < 0) a += md;
    int ans = 1 % md;
    for (char c : b) {
        assert('0' <= c && c <= '9');
        ans = ModArith64::mul(ans, ModArith64::power(ans, 9, md), md);
        if (c != '0') ans = ModArith64::mul(ans, ModArith64::power(a, c - '0', md), md);
    }
    return ans;
}

inline int solve(const string& a, const string& b, int md) {
    return solve(askMod(a, md), b, md);
}
}  // namespace BigPow
