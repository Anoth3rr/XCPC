struct BIT {
    int n;
    vector<int> w;
    BIT(int n) : n(n), w(n + 1) {}

    void add(int x, int v) {
        for (; x <= n; x += x & -x) w[x] += v;
    }

    int ask(int l, int r) {
        auto sum = [&](int x) {
            int ans = 0;
            for (; x; x -= x & -x) ans += w[x];
            return ans;
        };
        return sum(r) - sum(l - 1);
    }
};