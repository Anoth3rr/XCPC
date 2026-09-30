struct exBIT {
    int n;
    vector<int> a, b;
    exBIT(int n) : n(n), a(n + 1), b(n + 1) {}

    void add(int l, int r, int v) {
        auto Add = [&](int x, int v) {
            for (int i = x; i <= n; i += i & -i) {
                a[i] += v;
                b[i] += x * v;
            }
        };
        Add(l, v), Add(r + 1, -v);
    }
    
    int ask(int l, int r) {
        auto sum = [&](int x) {
            int ans = 0;
            for (int i = x; i; i -= i & -i) ans += (x + 1) * a[i] - b[i];
            return ans;
        };
        return sum(r) - sum(l - 1);
    }
};