struct Comb {
    struct Node {
        int p, pk, e;
        vector<int> f;
    };
    int P;
    vector<Node> a;
    Comb(int p) : P(p) {
        int x = P;
        for (int p = 2; p * p <= x; ++p) {
            if (x % p) continue;

            int pk = 1, e = 0;
            while (x % p == 0) {
                x /= p;
                pk *= p;
                ++e;
            }
            add(p, pk, e);
        }
        if (x > 1) add(x, x, 1);
    }
    static int mul(int a, int b, int m) {
        a %= m, b %= m;
        int r = a * b - m * (int)(1.0L / m * a * b);
        return r - m * (r >= m) + m * (r < 0);
    }
    static int power(int a, int b, int m) {
        int r = 1 % m;
        for (; b; b >>= 1, a = mul(a, a, m))
            if (b & 1) r = mul(r, a, m);
        return r;
    }
    static int inv(int a, int m) {
        int b = m, x = 1, y = 0;
        while (b) {
            int q = a / b, r = a % b;
            a = b, b = r;
            int z = x - q * y;
            x = y, y = z;
        }
        return (x % m + m) % m;
    }
    void add(int p, int pk, int e) {
        Node x{p, pk, e, vector<int>(pk + 1)};
        x.f[0] = 1;
        for (int i = 1; i <= pk; ++i) {
            x.f[i] = mul(x.f[i - 1], i % p ? i : 1, pk);
        }
        a.push_back(move(x));
    }
    static int vp(int n, int p) {
        int res = 0;
        while (n) n /= p, res += n;
        return res;
    }

    int fact(int n, const Node &x) const {
        if (!n) return 1;
        return mul(mul(power(x.f[x.pk], n / x.pk, x.pk), x.f[n % x.pk], x.pk), fact(n / x.p, x), x.pk);
    }

    int Cpk(int n, int m, const Node &x) const {
        int e = vp(n, x.p) - vp(m, x.p) - vp(n - m, x.p);
        if (e >= x.e) return 0;

        int res = fact(n, x);
        res = mul(res, inv(fact(m, x), x.pk), x.pk);
        res = mul(res, inv(fact(n - m, x), x.pk), x.pk);
        return mul(res, power(x.p, e, x.pk), x.pk);
    }

    int C(int n, int m) const {
        if (P == 1 || m < 0 || m > n) return 0;
        m = min(m, n - m);

        int ans = 0;
        for (const auto &x : a) {
            int M = P / x.pk;
            ans = (ans + mul(mul(Cpk(n, m, x), M, P), inv(M % x.pk, x.pk), P)) % P;
        }
        return ans;
    }
};