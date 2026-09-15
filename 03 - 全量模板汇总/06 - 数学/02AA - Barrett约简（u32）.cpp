/*
用途：固定 u32 模数下的 Barrett 模乘，避免每次 64 位除法。
用法：Barrett bt(p)；bt.mul(a,b)，其中 1 <= p < 2^31，0 <= a,b < p。
复杂度：O(1)。适合运行时 u32 模数且乘法极多的卡常环境。
性能：u32/u64/u128 是本模板为确定字宽和乘高位而保留的必要例外。
*/

using u32 = uint32_t;
using u64 = uint64_t;
using u128 = __uint128_t;

struct Barrett {
    u32 md;
    u64 im;

    explicit Barrett(u32 md) : md(md), im(u64(-1) / md + 1) {
        assert(0 < md && md < (u32(1) << 31));
    }

    u32 mul(u32 a, u32 b) const {
        u64 z = u64(a) * b;
        u64 q = u128(z) * im >> 64;
        u32 r = z - q * md;
        if (r >= md) r += md;
        return r;
    }
};
