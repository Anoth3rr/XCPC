int gcd(int a, int b) {
    if (!a || !b) return a | b;
    int t = __builtin_ctzll(a | b);
    a >>= __builtin_ctzll(a);
    while (b) {
        b >>= __builtin_ctzll(b);
        if (a > b) swap(a, b);
        b -= a;
    }
    return a << t;
}
