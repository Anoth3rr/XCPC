/*
用途：用 Machin 公式计算圆周率。
依赖：高精度 01A（或 01B）BigInt + 02A FixedPoint。适合约 2 * 10^3 位及以下；更高位数使用 10B。
*/

FixedPoint machin(int n) {
    const int ex = 40;
    const int pre = n + ex;
    FixedPoint::setPre(pre);

    auto arc = [&](int iv) -> FixedPoint {
        FixedPoint trm = FixedPoint(1) / FixedPoint(iv);
        FixedPoint sum = trm;
        int sgn = -1;
        FixedPoint iv2 = FixedPoint(1) / FixedPoint(iv * iv);
        FixedPoint eps(BigInt(1), pre + 15, 1);
        for (int k = 3;; k += 2) {
            trm = trm * iv2;
            FixedPoint cur = trm / FixedPoint(k);
            if (cur.abs() < eps) break;
            if (sgn > 0)
                sum = sum + cur;
            else
                sum = sum - cur;
            sgn = -sgn;
        }
        sum.round(FixedPoint::pre);
        sum.norm();
        return sum;
    };

    FixedPoint a1 = arc(5);
    FixedPoint a2 = arc(239);
    FixedPoint pi = a1 * FixedPoint(16) - a2 * FixedPoint(4);
    pi.round(n);
    pi.norm();
    return pi;
}
