void solve() {
    int n;
    cin >> n;
    int ans = 0;
    for (int i = 1, j; i <= n; i = j + 1) {
        j = n / (n / i);
        ans += (j - i + 1) * (n / i);
    }
    cout << ans << endl;
}
