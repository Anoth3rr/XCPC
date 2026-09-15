/*
用途：64 位整数确定性素性检验。
复杂度：O(log n) 次模乘；下列底数对 u64 范围确定正确。
环境：GCC/Clang 用 int 为 O(1) 模乘；其他编译器自动退化到 O(log mod) 加法模乘。
*/

using u64 = uint64_t;
using u128 = __uint128_t;

namespace Miller {

inline u64 mul(u64 a, u64 b, u64 mod) {
    assert(mod > 0);
    a %= mod;
    b %= mod;
#ifdef __SIZEOF_INT128__
    return u128(a) * b % mod;
#else
    u64 res = 0;
    while (b > 0) {
        if (b & 1) res = res >= mod - a ? res - (mod - a) : res + a;
        a = a >= mod - a ? a - (mod - a) : a + a;
        b >>= 1;
    }
    return res;
#endif
}

inline u64 power(u64 a, u64 e, u64 mod) {
    u64 res = 1 % mod;
    while (e > 0) {
        if (e & 1) res = mul(res, a, mod);
        a = mul(a, a, mod);
        e >>= 1;
    }
    return res;
}

inline bool ask(u64 n) {
    if (n < 2) return false;
    for (u64 p : {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL}) {
        if (n % p == 0) return n == p;
    }
    u64 d = n - 1;
    int s = 0;
    while ((d & 1) == 0) d >>= 1, ++s;
    for (u64 a : {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL}) {
        if (a % n == 0) continue;
        u64 x = power(a % n, d, n);
        if (x == 1 || x == n - 1) continue;
        bool ok = true;
        for (int r = 1; r < s; ++r) {
            x = mul(x, x, n);
            if (x == n - 1) {
                ok = false;
                break;
            }
        }
        if (ok) return false;
    }
    return true;
}
}  // namespace Miller
