vector<int> primes, spf(maxn + 1);

void init() {
    for (int i = 2; i <= maxn; ++i) {
        if (!spf[i])
            primes.push_back(i), spf[i] = i;
        for (int j = 0; primes[j] * i <= maxn; ++j) {
            int m = primes[j] * i;
            spf[m] = primes[j];
            if (i % primes[j] == 0)
                break;
        }
    }
}