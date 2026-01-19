constexpr int maxn = 1e7;

unordered_map<int, Z> fMu;

vector<int> spf(maxn + 1), primes, phi(maxn + 1), mu(maxn + 1);
vector<ll> sphi(maxn + 1);

void init(int n) {
    phi[1] = 1, mu[1] = 1;
    primes.push_back(1);

    for (int i = 2; i <= maxn; i++) {
        if (spf[i] == 0)
            spf[i] = i, phi[i] = i - 1, mu[i] = -1, primes.push_back(i);

        for (int j = 1; primes[j] * i <= maxn; j++) {
            int m = primes[j] * i;
            spf[m] = primes[j];

            if (i % primes[j] == 0) {
                phi[m] = phi[i] * primes[j];
                break;
            }
            phi[m] = phi[i] * (primes[j] - 1);
            mu[m] = -mu[i];
        }
    }

    for (int i = 1; i <= n; i++) {
        sphi[i] = sphi[i - 1] + phi[i];
        mu[i] += mu[i - 1];
    }
}

Z sumMu(int n) {
    if (n <= maxn) {
        return mu[n];
    }
    if (fMu.count(n)) {
        return fMu[n];
    }
    if (n == 0) {
        return 0;
    }
    Z ans = 1;
    int l = 2, r;
    while (l <= n) {
        r = n / (n / l);
        ans -= (r - l + 1) * sumMu(n / l);
        l = r + 1;
    }
    return ans;
}