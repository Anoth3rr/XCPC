/*
用途：常用位操作、子掩码与超掩码枚举。
前提：mask 使用 u64；超集枚举要求 bitCount < 64，实际通常 <= 20。
复杂度：子掩码枚举 O(2^popcount(mask))，所有 mask 的子掩码总复杂度 O(3^n)。
*/

using u64 = uint64_t;

namespace BitToolkit {
inline u64 lowbit(u64 val) {
    return val & (0ULL - val);
}

inline int bitLen(u64 val) {
    return val == 0 ? 0 : 64 - __builtin_clzll(val);
}

inline int ctz(u64 val) {
    return val == 0 ? 64 : __builtin_ctzll(val);
}

template <class F> void submask(u64 msk, F vis) {
    u64 s = msk;
    while (true) {
        vis(s);
        if (s == 0) break;
        s = (s - 1) & msk;
    }
}

template <class F> void supset(u64 msk, int k, F vis) {
    assert(0 <= k && k < 64 && (msk >> k) == 0);
    u64 all = (1ULL << k) - 1;
    submask(all ^ msk, [&](u64 c) { vis(msk | c); });
}

inline u64 nxtComb(u64 val) {
    assert(val != 0);
    u64 lb = lowbit(val);
    u64 nxt = val + lb;
    assert(nxt > val);
    return nxt | (((val ^ nxt) / lb) >> 2);
}
}  // namespace BitToolkit
