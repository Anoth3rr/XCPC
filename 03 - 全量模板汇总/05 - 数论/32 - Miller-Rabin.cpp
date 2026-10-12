int mul(int a, int b, int m) {
    return i128(a) * b % m;
}

bool isprime(int n) {
    if (n < 2 || n % 6 % 4 != 1) return (n | 1) == 3;
    int s = __builtin_ctzll(n - 1), d = n >> s;
    for (int a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) {
        if (!(a %= n)) continue;
        int x = 1;
        for (int b = d; b; b >>= 1, a = mul(a, a, n))
            if (b & 1) x = mul(x, a, n);
        if (x == 1 || x == n - 1) continue;
        for (int i = 1; i < s && x != n - 1; ++i) x = mul(x, x, n);
        if (x != n - 1) return false;
    }
    return true;
}
