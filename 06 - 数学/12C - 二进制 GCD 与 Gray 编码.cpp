/*
用途：Stein（二进制）gcd，以及 Gray 编码互转。
复杂度：binaryGcd 为 O(log max(a,b))；Gray 编码/解码 O(1) / O(log word bits)。
*/

using u64 = uint64_t;

namespace BinArith {
inline u64 gcd(u64 l, u64 r) {
    if (l == 0) return r;
    if (r == 0) return l;
    int s = __builtin_ctzll(l | r);
    l >>= __builtin_ctzll(l);
    do {
        r >>= __builtin_ctzll(r);
        if (l > r) swap(l, r);
        r -= l;
    } while (r != 0);
    return l << s;
}

inline u64 gray(u64 val) {
    return val ^ (val >> 1);
}

inline u64 igray(u64 g) {
    for (int sh = 1; sh < 64; sh <<= 1) g ^= g >> sh;
    return g;
}
}  // namespace BinArith
