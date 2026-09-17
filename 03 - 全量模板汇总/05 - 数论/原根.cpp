namespace PrimRoot {
    inline int power(int a, int b, int m) {
        int r = 1 % m;
        for (; b; b >>= 1, a = a * a % m)
            if (b & 1) r = r * a % m;
        return r;
    }
    inline vector<int> fact(int x) {
        vector<int> r;
        for (int p = 2; p * p <= x; ++p) {
            if (x % p == 0) {
                while (x % p == 0) x /= p;
                r.push_back(p);
            }
        }
        if (x != 1) r.push_back(x);
        return r;
    }
    inline bool exist(int mod) {
        if (mod == 2 || mod == 4) return true;
        int x = mod, c = 0;
        while (x % 2 == 0) x /= 2, ++c;
        if (c > 1 || x == 1) return false;
        int p = 0;
        for (int d = 3; d <= x / d; d += 2) {
            if (x % d == 0) {
                p = d;
                break;
            }
        }
        if (!p) return true;
        while (x % p == 0) x /= p;
        return x == 1;
    }
    inline int phi(int mod) {
        int res = mod;
        for (auto p : fact(mod)) res = res / p * (p - 1);
        return res;
    }

    inline bool ask(int g, int mod, int ph, const vector<int> &fac) {
        g %= mod;
        if (g <= 0) g += mod;
        if (gcd(g, mod) != 1) return false;
        for (int p : fac) {
            if (power(g, ph / p, mod) == 1) return false;
        }
        return true;
    }

    inline bool ask(int g, int mod) {
        if (!exist(mod)) return false;
        int ph = phi(mod);
        return ask(g, mod, ph, fact(ph));
    }

    inline int find(int mod, int ph, const vector<int> &fac) {
        for (int g = 1; g < mod; ++g) {
            if (ask(g, mod, ph, fac)) return g;
        }
        return -1;
    }

    inline int solve(int mod) {
        if (!exist(mod)) return -1;
        int ph = phi(mod);
        return find(mod, ph, fact(ph));
    }

    inline vector<int> roots(int mod) {
        if (!exist(mod)) return {};
        int ph = phi(mod);
        vector<int> fac = fact(ph);
        int g = find(mod, ph, fac), cnt = phi(ph);
        vector<int> res;
        for (int k = 1, x = g; k <= ph; ++k) {
            if (gcd(k, ph) == 1) res.push_back(x);
            x = x * g % mod;
        }
        sort(res.begin(), res.end());
        return res;
    }
} // namespace PrimRoot