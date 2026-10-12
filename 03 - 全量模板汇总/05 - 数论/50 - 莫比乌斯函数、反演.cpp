const int N = 5e4 + 10;
bool st[N];
int mu[N], prime[N], cnt, sum[N];
void getMu() {
    mu[1] = 1;
    for (int i = 2; i <= N - 10; i++) {
        if (!st[i]) {
            prime[++cnt] = i;
            mu[i] = -1;
        }
        for (int j = 1; j <= cnt && i * prime[j] <= N - 10; j++) {
            st[i * prime[j]] = true;
            if (i % prime[j] == 0) {
                mu[i * prime[j]] = 0;
                break;
            }
            mu[i * prime[j]] = -mu[i];
        }
    }
    for (int i = 1; i <= N - 10; i++) {
        sum[i] = sum[i - 1] + mu[i];
    }
}
void solve() {
    int n, m, k; cin >> n >> m >> k;
    n = n / k, m = m / k;
    if (n < m) swap(n, m);
    LL ans = 0;
    for (int i = 1, j = 0; i <= m; i = j + 1) {
        j = min(n / (n / i), m / (m / i));
        ans += (LL)(sum[j] - sum[i - 1]) * (n / i) * (m / i);
    }
    cout << ans << "\n";
}
int main() {
    getMu();
    int T; cin >> T;
    while (T--) solve();
}
