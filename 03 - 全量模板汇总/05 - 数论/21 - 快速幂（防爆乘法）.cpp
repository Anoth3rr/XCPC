int mul(int a, int b, int m) {
    int r = i128(a) * b % m;
    return r < 0 ? r + m : r;
}

int power(int a, int b, int m) {
    int r = 1 % m;
    a %= m;
    for (; b; b >>= 1, a = mul(a, a, m))
        if (b & 1) r = mul(r, a, m);
    return r;
}
