/*
用途：数值计算定积分 ∫[l,r] f(x) dx。
前提：函数在区间上足够光滑；奇点、极高频振荡、间断点应先分段处理。
复杂度：自适应递归，eps 为绝对误差目标，maxDepth 防止病态数据无限递归。
*/
using ld = long double;

namespace Simpson {
template <class F>
ld solve(F f, ld l, ld r, ld eps = 1e-10L, int dep = 30) {
    assert(eps > 0 && dep >= 0);
    if (l > r) return -solve(f, r, l, eps, dep);
    auto sp = [](ld a, ld b, ld x, ld y, ld z) {
        return (b - a) * (x + 4 * y + z) / 6;
    };

    ld mid = (l + r) / 2;
    ld fl = f(l), fm = f(mid), fr = f(r);
    ld s = sp(l, r, fl, fm, fr);
    std::function<ld(ld, ld, ld, ld, ld, ld, ld, int)> dfs;
    dfs = [&](ld a, ld b, ld x, ld y, ld z, ld v,
              ld e, int d) -> ld {
        ld m = (a + b) / 2;
        ld lm = (a + m) / 2, rm = (m + b) / 2;
        ld xl = f(lm), xr = f(rm);
        ld sl = sp(a, m, x, xl, y);
        ld sr = sp(m, b, y, xr, z);
        ld err = sl + sr - v;
        if (d == 0 || fabsl(err) <= 15 * e) {
            return sl + sr + err / 15;
        }
        return dfs(a, m, x, xl, y, sl, e / 2, d - 1) +
               dfs(m, b, y, xr, z, sr, e / 2, d - 1);
    };
    return dfs(l, r, fl, fm, fr, s, eps, dep);
}
}  // namespace Simpson
