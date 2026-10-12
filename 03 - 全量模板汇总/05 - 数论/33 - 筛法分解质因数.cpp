constexpr int N = 1E7;

vector<int> primes, spf(N + 1), maxp(N + 1);

void init() {
    primes.clear();
    for (int i = 2; i <= N; i++) {
        if (!spf[i]) spf[i] = maxp[i] = i, primes.push_back(i);
        for (auto p : primes) {
            if (p > spf[i] || p > N / i) break;
            spf[i * p] = p;
            maxp[i * p] = maxp[i];
        }
    }
}

vector<pii> fact(int n) {
    vector<pii> r;
    while (n > 1) {
        int p = spf[n], e = 0;
        while (n % p == 0) n /= p, e++;
        r.push_back({p, e});
    }
    return r;
}
