signed main() {
    int n, k;
    cin >> n >> k;
    
    int cnt = __builtin_popcountll(n);
    
    if (k < cnt || n < k) {
        cout << "NO\n";
        return 0;
    }
    cout << "YES\n";
    
    vector<int> num;
    while (n) {
        num.push_back(n % 2);
        n /= 2;
    }
    
    for (int i = num.size() - 1; i > 0; i--) {
        int p = min(k - cnt, num[i]);
        num[i] -= p;
        num[i - 1] += 2 * p;
        cnt += p;
    }
    
    for (int i = 0; i < num.size(); i++) {
        for (int j = 1; j <= num[i]; j++) {
            cout << (1LL << i) << " ";
        }
    }
}
