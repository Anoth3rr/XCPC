/*
用途：群作用下的轨道数；f(i) 返回第 i 个变换的不动点数。
前提：群大小在 Z 中可逆。
*/
namespace Burnside {
template <class Z, class F> Z burn(int n, F f) {
    assert(n > 0);
    Z s = 0;
    for (int i = 0; i < n; ++i) s += f(i);
    return s / Z(n);
}
}  // namespace Burnside
