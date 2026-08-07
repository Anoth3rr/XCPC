/*
用途：64 位安全模乘、模幂与费马逆元。
前提：inv 仅在 mod 为质数且 a 非 0 (mod mod) 时使用。
环境：GCC/Clang 的 int 为 O(log e)；无 int 时模乘退化为 O(log mod)。
*/

using u64 = uint64_t;
using u128 = __uint128_t;

namespace ModArith64 {

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
    assert(mod > 0);
    u64 res = 1 % mod;
    while (e > 0) {
        if (e & 1) res = mul(res, a, mod);
        a = mul(a, a, mod);
        e >>= 1;
    }
    return res;
}

inline u64 inv(u64 a, u64 mod) {
    assert(mod > 1 && a % mod != 0);
    return power(a % mod, mod - 2, mod);
}
}  // namespace ModArith64
