/*
用途：求 x^2 = a (mod p) 的一组解；p 必须为质数。
依赖：02A - 自动取模（MInt）。无解返回 nullopt。
*/

template <class Z> optional<Z> sqrt(Z a) {
    int p = Z::askMod();
    assert(p >= 2);
    if (a == Z(0) || p == 2) return a;
    if (a.pow((p - 1) / 2) != Z(1)) return nullopt;
    if (p % 4 == 3) return a.pow((p + 1) / 4);

    int s = 0;
    int q = p - 1;
    while (!(q & 1)) q >>= 1, ++s;
    Z z = 2;
    while (z.pow((p - 1) / 2) == Z(1)) ++z;
    Z x = a.pow((q + 1) / 2), b = a.pow(q), c = z.pow(q);
    int m = s;
    while (b != Z(1)) {
        int i = 1;
        Z d = b * b;
        while (i < m && d != Z(1)) d *= d, ++i;
        assert(i < m);
        Z t = c.pow(1LL << (m - i - 1));
        x *= t;
        c = t * t;
        b *= c;
        m = i;
    }
    return x;
}
