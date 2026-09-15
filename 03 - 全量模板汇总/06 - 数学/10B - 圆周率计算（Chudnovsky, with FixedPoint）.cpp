/*
用途：用 Chudnovsky 公式计算圆周率。
依赖：高精度 01B BigInt + 02A FixedPoint。适合约 2 * 10^5 位及以下；低精度时 10A 常数更小。
*/
FixedPoint chud(int n) {
    const int ex = 20;
    const int pre = n + ex;
    FixedPoint::setPre(pre);

    auto fac = [](int x) -> BigInt {
        BigInt res(1);
        for (int i = 2; i <= x; ++i) res *= BigInt(i);
        return res;
    };
    auto tf = [](const BigInt& x) -> FixedPoint {
        return FixedPoint(x, 0, 1);
    };
    auto sf = [&](const FixedPoint& x) -> FixedPoint {
        FixedPoint y(to_string(sqrt(stod(x.str()))));
        for (int i = 0; i < 80; ++i) {
            y = (y + x / y) / FixedPoint(2);
        }
        y.round(pre);
        y.norm();
        return y;
    };

    FixedPoint sum(0);
    string es = "0." + string(pre + 5, '0') + "1";
    FixedPoint eps(es);

    for (int k = 0;; ++k) {
        BigInt a = fac(6 * k) * (BigInt(545140134) * k + BigInt(13591409));
        BigInt d = fac(3 * k) * BigInt::pow(fac(k), 3) * BigInt::pow(BigInt(640320), 3 * k);
        FixedPoint trm = tf(a) / tf(d);
        if (k % 2 == 1) trm = FixedPoint(0) - trm;
        sum += trm;
        if (trm.abs() < eps) break;
    }

    FixedPoint pi = FixedPoint(426880) * sf(FixedPoint(10005)) / sum;
    pi.round(n);
    pi.norm();
    return pi;
}
