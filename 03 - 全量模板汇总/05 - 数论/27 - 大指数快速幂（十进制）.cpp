int mypow10(int n, vector<int> k, int p) {
    int r = 1;
    for (int i = k.size() - 1; i >= 0; i--) {
        for (int j = 1; j <= k[i]; j++) {
            r = r * n % p;
        }
        int v = 1;
        for (int j = 0; j <= 9; j++) {
            v = v * n % p;
        }
        n = v;
    }
    return r;
}
signed main() {
    string n_, k_;
    int p;
    cin >> n_ >> k_ >> p;
    
    int n = 0; // 转化并计算 n % p
    for (auto it : n_) {
        n = n * 10 + it - '0';
        n %= p;
    }
    vector<int> k; // 转化 k
    for (auto it : k_) {
        k.push_back(it - '0');
    }
    cout << mypow10(n, k, p) << endl; // 暴力快速幂
}
