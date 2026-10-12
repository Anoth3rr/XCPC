BigInt gcd(BigInt a, BigInt b) {
    a = a.abs(), b = b.abs();
    if (a.isZero() || b.isZero()) return a + b;
    int k = 0;
    while (a % 2 == 0 && b % 2 == 0) a /= 2, b /= 2, ++k;
    while (a % 2 == 0) a /= 2;
    do {
        while (b % 2 == 0) b /= 2;
        if (a > b) swap(a, b);
        b -= a;
    } while (!b.isZero());
    while (k--) a *= 2;
    return a;
}
