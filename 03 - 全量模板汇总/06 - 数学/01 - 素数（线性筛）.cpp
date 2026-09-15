struct Sieve {
    int n = 0;
    vector<int> pri, lp, phi, mu;

    Sieve() = default;
    explicit Sieve(int n) {
        init(n);
    }

    void init(int _n) {
        assert(_n >= 0);
        n = _n;
        pri.clear();
        lp.assign(n + 1, 0);
        phi.assign(n + 1, 0);
        mu.assign(n + 1, 0);
        if (n >= 1) {
            lp[1] = phi[1] = mu[1] = 1;
        }
        for (int x = 2; x <= n; ++x) {
            if (lp[x] == 0) {
                lp[x] = x;
                pri.push_back(x);
                phi[x] = x - 1;
                mu[x] = -1;
            }
            for (int p : pri) {
                if (p > n / x) break; // 避免 p * x 溢出。
                int y = p * x;
                lp[y] = p;
                if (x % p == 0) {
                    phi[y] = phi[x] * p;
                    mu[y] = 0;
                    break;
                }
                phi[y] = phi[x] * (p - 1);
                mu[y] = -mu[x];
            }
        }
    }

    bool ask(int x) const {
        return 2 <= x && x <= n && lp[x] == x;
    }

    vector<pair<int, int>> factor(int x) const {
        assert(1 <= x && x <= n);
        vector<pair<int, int>> res;
        while (x > 1) {
            int p = lp[x], e = 0;
            do {
                x /= p;
                ++e;
            } while (x % p == 0);
            res.push_back({p, e});
        }
        return res;
    }
};
