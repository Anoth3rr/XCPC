/*
用途：在单峰/单谷函数上找最小值点；实数版返回近似 argmin，整数版返回精确 argmin。
前提：目标函数在给定闭区间上单峰（或单谷）；不满足时不能套用。
复杂度：实数版 O(iterations)，整数版 O(log(r-l)) 次后常数枚举。
*/
using ld = long double;

namespace Ternary {
template <class F>
ld argmin(ld l, ld r, F f, int lim = 200) {
    assert(l <= r && lim > 0);
    for (int it = 0; it < lim; ++it) {
        ld fst = (2 * l + r) / 3;
        ld b = (l + 2 * r) / 3;
        if (f(fst) <= f(b)) r = b;
        else l = fst;
    }
    return (l + r) / 2;
}

template <class F>
int argmin(int l, int r, F f) {
    assert(l <= r);
    while (r - l > 3) {
        int fst = l + (r - l) / 3;
        int b = r - (r - l) / 3;
        if (f(fst) <= f(b)) r = b - 1;
        else l = fst + 1;
    }
    int ans = l;
    for (int val = l + 1; val <= r; ++val) {
        if (f(val) < f(ans)) ans = val;
    }
    return ans;
}
}  // namespace Ternary
