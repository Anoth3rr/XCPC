void solve() {
    int n;
    cin >> n;
    vector<int> p(n);
    for (auto &v : p) cin >> v;

    int ans = 0;
    for (int mask = 1; mask < (1LL << n); ++mask) {
        int x = 1, cnt = 0;
        bool ok = true;
        for (int i = 0; i < n; ++i)
            if (mask >> i & 1) {
                if (x > n / p[i]) {
                    ok = false;
                    break;
                }
                x *= p[i];
                ++cnt;
            }

        if (ok) ans += (cnt & 1 ? 1 : -1) * (n / x);
    }
    cout << ans << endl;
}