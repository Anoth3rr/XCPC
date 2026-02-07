Z Sn2(Z n) {
    return n * (n + 1) * (2 * n + 1) / 6;
}

unordered_map<int, Z> fMu;

vector<int> spf(maxn + 1), primes, phi(maxn + 1), mu(maxn + 1);
vector<Z> smu(maxn + 1);

Z sumMu(int n) {
    if (n <= maxn) {
        return smu[n];
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
        ans -= (Sn2(Z(r)) - Sn2(Z(l - 1))) * sumMu(n / l);
        l = r + 1;
    }
    return fMu[n] = ans;
}

void init() {
    mu[1] = 1, smu[1] = 1;

    for (int i = 2; i <= maxn; i++) {
        if (!spf[i])
            spf[i] = i, mu[i] = -1, primes.push_back(i);

        for (int j = 0; primes[j] * i <= maxn; j++) {
            int m = primes[j] * i;
            spf[m] = primes[j];

            if (i % primes[j] == 0) {
                mu[m] = 0;
                break;
            }
            mu[m] = -mu[i];
        }

        if (mu[i] == 1)
            smu[i] = smu[i - 1] + Z(i) * i;
        else if (mu[i] == -1)
            smu[i] = smu[i - 1] - Z(i) * i;
        else
            smu[i] = smu[i - 1];
    }
}