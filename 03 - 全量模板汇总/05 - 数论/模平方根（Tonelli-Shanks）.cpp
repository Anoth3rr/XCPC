int sqrt(int a, int p) {
    a %= p;
    if (a == 0 || p == 2) return a;
    if (power(a, (p - 1) / 2, p) != 1) return -1;
    if (p % 4 == 3) return power(a, (p + 1) / 4, p);

    int s = 0, q = p - 1;
    while (!(q & 1)) q >>= 1, ++s;
    int z = 2;
    while (power(z, (p - 1) / 2, p) == 1) ++z;
    int x = power(a, (q + 1) / 2, p), b = power(a, q, p), c = power(z, q, p);
    int m = s;
    while (b != 1) {
        int i = 1;
        int d = b * b % p;
        while (i < m && d != 1) d = d * d % p, ++i;
        assert(i < m);
        int t = power(c, 1LL << (m - i - 1), p);
        x = x * t % p;
        c = t * t % p;
        b = b * c % p;
        m = i;
    }
    return x;
}