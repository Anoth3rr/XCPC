constexpr int N = 1E7;

vector<int> primes, spf(N + 1);

void init() {
    primes.clear();
    fill(spf.begin(), spf.end(), 0);
    for (int i = 2; i <= N; i++) {
        if (!spf[i]) spf[i] = i, primes.push_back(i);
        for (auto p : primes) {
            if (p > N / i) break;
            int y = p * i;
            spf[y] = p;
            if (p == spf[i]) break;
        }
    }
}

bool isprime(int n) {
    return n >= 2 && spf[n] == n;
}
