/*
用途：Cipolla 算法求 x^2 = n (mod p) 的一个解；无解返回 nullopt。
依赖：02A - 自动取模（MInt）。p 必须为质数。
区别：02E 是确定性的 Tonelli-Shanks；本模板随机选非二次剩余，适合希望保留 Cipolla 写法时使用。
*/

template <class Z> optional<Z> cipolla(Z n) {
    int p = Z::askMod();
    assert(p >= 2);
    if (p == 2 || n == Z(0)) return n;
    if (n.pow((p - 1) / 2) != Z(1)) return nullopt;

    static mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    Z a, w;
    do {
        a = Z(rng() % p);
        w = a * a - n;
    } while (w.pow((p - 1) / 2) != Z(p - 1));

    auto mul = [&](array<Z, 2> x, array<Z, 2> y) {
        return array<Z, 2>{x[0] * y[0] + x[1] * y[1] * w,
                           x[0] * y[1] + x[1] * y[0]};
    };
    array<Z, 2> r{Z(1), Z(0)}, b{a, Z(1)};
    for (int k = (p + 1) / 2; k; k >>= 1) {
        if (k & 1) r = mul(r, b);
        b = mul(b, b);
    }
    return r[0];
}
