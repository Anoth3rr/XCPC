vector<int> primes, spf(maxn + 1), phi(maxn + 1);

void init() {
    phi[1] = 1;
    for (int i = 2; i <= maxn; i++) {
        if (!spf[i])
            primes.push_back(i), phi[i] = i - 1;
        for (int j = 0; primes[j] * i <= maxn; ++j) {
            int m = primes[j] * i;
            spf[m] = primes[j];
            if (i % primes[j] == 0) {
                phi[m] = phi[i] * primes[j];
                break;
            }
            phi[m] = phi[i] * (p - 1);
        }
    }
}