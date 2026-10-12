auto L = [&](int n, int m, int p) {
    if (m < 0 || m > n) return 0LL;
    int r = 1;
    while (n || m) {
        int x = n % p, y = m % p;
        r = r * C(x, y, p) % p;
        n /= p, m /= p;
    }
    return r;
};
