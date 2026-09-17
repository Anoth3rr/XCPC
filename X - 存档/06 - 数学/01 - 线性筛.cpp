constexpr int N = 2e5;

vector<int> primes, spf(N + 1);
vector<int> phi(N + 1), mu(N + 1);

void init() {
    for (int i = 2; i <= N; ++i) {
        if (!spf[i]) {
            spf[i] = i, primes.push_back(i);
            phi[i] = i - 1, mu[i] = -1;
        }
        for (auto p : primes) {
            if (i * p > N) break;
            int y = p * i;
            spf[y] = p;
            if (p == spf[i]) {
                phi[y] = phi[i] * p;
                break;
            }
            phi[y] = phi[i] * (p - 1), mu[y] = -mu[i];
        }
    }
}

vector<pii> fact(int x) {
    vector<pii> res;
    while (x > 1) {
        int p = spf[x], e = 0;
        do {
            x /= p;
            ++e;
        } while (x % p == 0);
        res.push_back({p, e});
    }
    return res;
}