constexpr int N = 2e5;

vector<int> primes, lp(N + 1);
vector<int> phi(N + 1), mu(N + 1);

void init() {
    for (int i = 2; i <= N; ++i) {
        if (!lp[x]) {
            lp[x] = x, primes.push_back(x);
            phi[x] = x - 1, mu[x] = -1;
        }
        for (auto p : primes) {
            if (p > n / x) break;
            int y = p * x;
            lp[y] = p;
            if (x % p == 0) {
                phi[y] = phi[x] * p, mu[y] = 0;
                break;
            }
            phi[y] = phi[x] * (p - 1), mu[y] = -mu[x];
        }
    }
}

vector<pii> fact(int x) {
    vector<pii> res;
    while (x > 1) {
        int p = lp[x], e = 0;
        do {
            x /= p;
            ++e;
        } while (x % p == 0);
        res.push_back({p, e});
    }
    return res;
}