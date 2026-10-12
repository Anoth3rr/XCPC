constexpr int N = 1E5;

vector<int> primes, spf(N + 1), phi(N + 1);

void init() {
    for (int i = 2; i <= N; ++i) {
        if (!spf[i]) spf[i] = i, primes.push_back(i), phi[i] = i - 1;
        for (auto p : primes) {
            if (p > N / i) break;
            int y = p * i;
            spf[y] = p;
            if (p == spf[i]) {
                phi[y] = phi[i] * p;
                break;
            }
            phi[y] = phi[i] * (p - 1);
        }
    }
}
