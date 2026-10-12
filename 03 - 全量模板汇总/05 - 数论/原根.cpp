int power(int a, int b, int m) {
    int r = 1 % m;
    a %= m;
    for (; b; b >>= 1, a = a * a % m) if (b & 1) r = r * a % m;
    return r;
}
int Root(int n) {
    auto fact = [&](int v) {
        vector<int> fac;
        for (int i = 2; i <= v / i; ++i) {
            if (v % i != 0) continue;
            fac.push_back(i);
            do {
                v /= i;
            } while(v % i == 0);
        }
        if (v > 1) fac.push_back(v);
        return fac;
    };
    int phi = n;
    for (auto v : fact(n)) phi = phi / v * (v - 1);
    auto f = fact(phi);
    auto check = [&](int g) {
        if (gcd(g, n) != 1) return false;
        for (auto v : f) if (power(g, phi / v, n) == 1) return false;
        return true;
    };
    for (int g = 1; g < n; ++g) {
        if (check(g)) return g;
    }
    return -1;
}
