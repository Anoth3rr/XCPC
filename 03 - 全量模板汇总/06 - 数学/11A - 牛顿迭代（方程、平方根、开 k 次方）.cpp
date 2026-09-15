/*
用途：Newton-Raphson 数值求根，以及平方根、非负 k 次根。
前提：函数在根附近可导且导数不接近 0；失败返回 nullopt。浮点题应按量级设置 eps。
复杂度：收敛区间内通常二次收敛；远离根或多重根时不保证收敛。
*/
using ld = long double;

namespace Newton {
template <class F, class DF>
optional<ld> solve(F f, DF df, ld x,
                   ld eps = 1e-12L, int lim = 100) {
    assert(eps > 0 && lim > 0);
    ld cur = x;
    for (int it = 0; it < lim; ++it) {
        ld val = f(cur);
        ld slp = df(cur);
        if (!isfinite(val) || !isfinite(slp) || fabsl(slp) <= eps) return nullopt;
        ld nxt = cur - val / slp;
        if (!isfinite(nxt)) return nullopt;
        if (fabsl(nxt - cur) <= eps * max(ld(1), fabsl(nxt))) return nxt;
        cur = nxt;
    }
    return nullopt;
}

inline ld sqrt(ld val, ld eps = 1e-15L) {
    assert(val >= 0);
    if (val == 0) return 0;
    ld cur = max(ld(1), val);
    for (int it = 0; it < 200; ++it) {
        ld nxt = (cur + val / cur) / 2;
        if (fabsl(nxt - cur) <= eps * max(ld(1), fabsl(nxt))) return nxt;
        cur = nxt;
    }
    return cur;
}

inline ld root(ld val, int k, ld eps = 1e-15L) {
    assert(val >= 0 && k >= 1);
    if (val == 0 || k == 1) return val;
    ld cur = max(ld(1), powl(val, 1.0L / k));
    for (int it = 0; it < 200; ++it) {
        ld nxt = (ld(k - 1) * cur + val / powl(cur, k - 1)) / k;
        if (fabsl(nxt - cur) <= eps * max(ld(1), fabsl(nxt))) return nxt;
        cur = nxt;
    }
    return cur;
}
}  // namespace Newton
