int rho(int n) {
    if (n % 2 == 0) return 2;
    int x = 0, y = 0, c = 0, p = 1, d;
    auto f = [&](int x) {
        int v = mul(x, x, n) - c;
        return v < 0 ? v + n : v;
    };
    for (int l = 1;; l *= 2) {
        if (x == y) c = rng() % (n - 1) + 1, y = 0, l = 1;
        x = y;
        for (int i = 0; i < l; ++i) {
            y = f(y);
            if (x == y) break;
            if (int q = mul(p, abs(x - y), n)) p = q;
            if (i % 128 == 0 && (d = gcd(p, n)) > 1) return d;
        }
    }
}

vector<int> fact(int x) {
    vector<int> r;
    auto f = [&](auto f, int x) {
        if (x == 1) return;
        if (isprime(x)) return r.push_back(x);
        int y = rho(x);
        f(f, y), f(f, x / y);
    };
    f(f, x), sort(r.begin(), r.end());
    return r;
}
