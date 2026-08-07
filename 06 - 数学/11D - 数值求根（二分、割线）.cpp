/*
用途：连续函数的二分求根和割线法求根。
前提：二分要求端点异号或已有根；割线法需要足够好的两个初值。失败返回 nullopt。
复杂度：二分 O(log((r-l)/eps))；割线法通常超线性收敛但无全局保证。
*/
using ld = long double;

namespace Root {
template <class F>
optional<ld> bisect(F f, ld l, ld r, ld eps = 1e-12L,
                    int lim = 200) {
    assert(l <= r && eps > 0 && lim > 0);
    ld fl = f(l), fr = f(r);
    if (fabsl(fl) <= eps) return l;
    if (fabsl(fr) <= eps) return r;
    if ((fl < 0) == (fr < 0)) return nullopt;
    for (int it = 0; it < lim; ++it) {
        ld mid = (l + r) / 2;
        ld fm = f(mid);
        if (fabsl(fm) <= eps || r - l <= eps) return mid;
        if ((fl < 0) == (fm < 0)) l = mid, fl = fm;
        else r = mid, fr = fm;
    }
    return (l + r) / 2;
}

template <class F>
optional<ld> secant(F f, ld fst, ld b, ld eps = 1e-12L,
                    int lim = 100) {
    assert(eps > 0 && lim > 0);
    ld fa = f(fst), fb = f(b);
    for (int it = 0; it < lim; ++it) {
        ld d = fb - fa;
        if (fabsl(d) <= eps) return nullopt;
        ld nxt = b - fb * (b - fst) / d;
        if (!isfinite(nxt)) return nullopt;
        if (fabsl(nxt - b) <= eps * max(ld(1), fabsl(nxt))) return nxt;
        fst = b;
        fa = fb;
        b = nxt;
        fb = f(b);
    }
    return nullopt;
}
}  // namespace Root
