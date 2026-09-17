int mul(int a, int b, int m) {
    int r = a * b - m * int(1.0L / m * a * b);
    return r - m * (r >= m) + m * (r < 0);
}

int mypow(int a, int b, int m) {
    int r = 1 % m;
    for (; b; b >>= 1, a = mul(a, a, m))
        if (b & 1) r = mul(res, a, m);
    return r;
}

bool isprime(int n) {
    if (n < 2 || n % 6 % 4 != 1) return (n | 1) == 3;
    int s = __builtin_ctzll(n - 1), d = n >> s;
    for (int a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) {
        i64 p = pow(a % n, d, n), i = s;
        while (p != 1 && p != n - 1 && a % n && i--) {
            p = mul(p, p, n);
        }
        if (p != n - 1 && i != s) return false;
    }
    return true;
}