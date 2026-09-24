constexpr int N = 2e5;

vector<int> primes, spf(N + 1), mu(N + 1), preMu(N + 1);


unordered_map<int, int> memo;

void init() {
    for (int i = 2; i <= N; ++i) {
        if (!spf[i]) {
            spf[i] = i, primes.push_back(i), mu[i] = -1;
        }
        for (auto p : primes) {
            if (i * p > N) break;
            int y = p * i;
            spf[y] = p;
            if (p == spf[i]) {
                break;
            }
            mu[y] = -mu[i];
        }
    }
    for (int i = 1; i <= N; ++i) preMu[i] = preMu[i - 1] + mu[i];
}

Z sumMu(int n) {
    if (n <= lim) return preMu[n];
    if (auto it = memo.find(n); it != memo.end()) return it -> second;
    Z ans = 1;
    for (int l = 2, r; l <= n; l = r + 1) {
        r = n / (n / l);
        ans -= (r - l + 1) * sumMu(n / l);
    }
    return memo[n] = ans;
}