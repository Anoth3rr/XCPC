int phi(int n) {
    assert(n >= 1);
    int res = n;
    for (int p = 2; p <= n / p; ++p) {
        if (n % p != 0) continue;
        while (n % p == 0) n /= p;
        res = res / p * (p - 1);
    }
    if (n > 1) res = res / n * (n - 1);
    return res;
}