struct rBIT {
    int n, lo;
    vector<int> w;
    rBIT(int lo, int hi) : lo(lo), n(hi - lo + 1), w(n + 1) {}

    int id(int x) const {
        return x - lo + 1;
    }
    void add(int x, int v) {
        for (int p = id(x); p <= n; p += p & -p) w[p] += v;
    }
    int sum(int p) const {
        int ans = 0;
        for (; p; p -= p & -p) ans += w[p];
        return ans;
    }

    int kth(int k) const {
        int ans = 0;
        for (int i = 1 << __lg(n); i; i >>= 1) {
            int nxt = ans + i;
            if (nxt <= n && w[nxt] < k) {
                k -= w[nxt];
                ans = nxt;
            }
        }
        return lo + ans;
    }
    int rank(int x) const { return sum(id(x) - 1) + 1; }
    int prec(int x) const { return kth(rank(x) - 1); }
    int sufc(int x) const { return kth(sum(id(x)) + 1); }
};