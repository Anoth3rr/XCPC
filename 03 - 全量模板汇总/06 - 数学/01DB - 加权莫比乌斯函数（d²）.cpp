/*
用途：杜教筛求 S(n)=sum_{d<=n} mu(d)*d^2（模质数）。
这是 01D 的加权版本，不能用普通 M(n) 替代；常用于带 d^2 权值的整除分块式。
依赖：02A - 自动取模（MInt）。Mint 的模数应为大于 3 的质数。
*/

template <class Z> struct Mobius2 {
    int n = 0;
    vector<int> lp, pri, mu;
    vector<Z> pre;
    unordered_map<int, Z> mp;

    Mobius2() = default;
    explicit Mobius2(int lim) { init(lim); }

    void init(int lim) {
        assert(lim >= 1);
        n = lim;
        lp.assign(n + 1, 0);
        mu.assign(n + 1, 0);
        pri.clear();
        pre.assign(n + 1, Z(0));
        lp[1] = 1;
        mu[1] = 1;
        for (int x = 2; x <= n; ++x) {
            if (lp[x] == 0) lp[x] = x, mu[x] = -1, pri.push_back(x);
            for (int p : pri) {
                if (p > n / x) break;
                int y = p * x;
                lp[y] = p;
                if (x % p == 0) {
                    mu[y] = 0;
                    break;
                }
                mu[y] = -mu[x];
            }
        }
        for (int i = 1; i <= n; ++i) pre[i] = pre[i - 1] + Z(mu[i]) * Z(i) * i;
        mp.clear();
    }

    static Z sum2(int n) {
        Z x = Z(n);
        return x * (x + 1) * (x * 2 + 1) / Z(6);
    }

    Z ask(int x) {
        assert(x >= 0 && n > 0);
        if (x <= n) return pre[x];
        if (auto it = mp.find(x); it != mp.end()) return it->second;
        Z ans = 1;
        for (int l = 2, r; l <= x; l = r + 1) {
            int q = x / l;
            r = x / q;
            ans -= (sum2(r) - sum2(l - 1)) * ask(q);
        }
        return mp[x] = ans;
    }
};
