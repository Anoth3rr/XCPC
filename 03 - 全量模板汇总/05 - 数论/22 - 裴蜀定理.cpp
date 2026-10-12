void solve() {
    int n, g = 0;
    cin >> n;
    for (int i = 0, x; i < n; ++i){
        cin >> x;
        g = gcd(g, x);
    }
    cout << g << endl;
}
