/*
用途：求最小非负 x，使 a^x ≡ b (mod m)。
依赖：02 - 快速幂（ModArith64）和 02BA - 扩展欧几里得（inv）。
接口：bsgs 要求 gcd(a,m)=1；exBSGS 允许不互素。均返回 optional<int>。
复杂度：O(sqrt(m)) 时间和空间；模数需在 signed int 范围内。
*/
using ld = long double;

namespace BSGS {
inline int norm(int val, int mod) {
    val %= mod;
    return val < 0 ? val + mod : val;
}

inline optional<int> bsgs(int a, int b, int mod) {
    assert(mod > 0);
    if (mod == 1) return 0;
    a = norm(a, mod);
    b = norm(b, mod);
    if (b == 1) return 0;
    if (gcd(a, mod) != 1) return nullopt;

    int blk = sqrtl(ld(mod));
    while (blk < mod / blk || (blk == mod / blk && blk * blk < mod)) ++blk;

    unordered_map<int, int> bb;
    bb.reserve(blk * 2 + 1);
    int cur = 1;
    for (int j = 0; j < blk; ++j) {
        if (!bb.count(cur)) bb.emplace(cur, j);
        cur = ModArith64::mul(cur, a, mod);
    }

    auto ia = inv(a, mod);
    assert(ia.has_value());
    int gs = ModArith64::power(*ia, blk, mod);
    cur = b;
    for (int i = 0; i <= blk; ++i) {
        auto it = bb.find(cur);
        if (it != bb.end()) return i * blk + it->second;
        cur = ModArith64::mul(cur, gs, mod);
    }
    return nullopt;
}

inline optional<int> exBSGS(int a, int b, int mod) {
    assert(mod > 0);
    if (mod == 1) return 0;
    a = norm(a, mod);
    b = norm(b, mod);
    if (b == 1) return 0;

    int ban = 0, scl = 1;
    while (true) {
        int g = gcd(a, mod);
        if (g == 1) break;
        if (b == scl) return ban;
        if (b % g != 0) return nullopt;
        b /= g;
        mod /= g;
        ++ban;
        if (mod == 1) return ban;
        scl = ModArith64::mul(scl, a / g, mod);
    }

    auto is = inv(scl, mod);
    assert(is.has_value());
    auto rem = bsgs(a, ModArith64::mul(b, *is, mod), mod);
    return rem ? optional<int>(*rem + ban) : nullopt;
}
}  // namespace BSGS
