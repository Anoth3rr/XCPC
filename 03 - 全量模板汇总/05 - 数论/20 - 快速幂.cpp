int power(int a, int b, int m) {
    int r = 1 % m;
    a %= m;
    for (; b; b >>= 1, a = a * a % m) 
        if (b & 1) r = r * a % m;
    return r;
}
