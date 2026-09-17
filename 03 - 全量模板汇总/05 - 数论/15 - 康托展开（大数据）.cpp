struct BIT {
    int n{};
    vector<int> w;

    BIT(int _n) : n(_n), w(_n + 1) {}

    void add(int x, const int &k) {
        for (; x <= n; x += x & -x) {
            w[x] += k;
        }
    }
    int ask(int x) const {
        int ans = 0;
        for (; x; x -= x & -x) {
            ans += w[x];
        }
        return ans;
    }
    int ask(int x, int y) const {
        return ask(y) - ask(x - 1);
    }
};

void solve() {
    int n;
    cin >> n;
    BIT a(n);
    vector<int> f(n + 1);
    f[0] = 1;
    for (int i = 1; i <= n; i++) {
        f[i] = f[i - 1] * i % mod;
        a.add(i, 1);
    }
    int ans = 0;
    for (int i = 1; i <= n; i++) {
        int x;
        cin >> x;
        ans = (ans + a.query(1, x - 1) * f[n - i] % mod) % mod;
        a.add(x, -1);
    }
    cout << (ans + 1) % mod << endl;
}