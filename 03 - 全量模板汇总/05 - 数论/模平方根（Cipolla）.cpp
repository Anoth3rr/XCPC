int cipolla(int n, int p) {
    n %= p;
    if (p == 2 || n % p == 0) return n;
    if (power(n, (p - 1) / 2, p) != 1) return -1;

    static mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    int a, w;
    do {
        a = rng() % p;
        w = (a * a % p - n % p + p) % p;
    } while (power(w, (p - 1) / 2, p) != p - 1);

    auto mul = [&](array<int, 2> x, array<int, 2> y) { return array<int, 2>{(x[0] * y[0] % p + x[1] * y[1] % p * w % p) % p, (x[0] * y[1] % p + x[1] * y[0] % p) % p}; };
    array<int, 2> r{1, 0}, b{a, 1};
    for (int k = (p + 1) / 2; k; k >>= 1) {
        if (k & 1) r = mul(r, b);
        b = mul(b, b);
    }
    return r[0];
}