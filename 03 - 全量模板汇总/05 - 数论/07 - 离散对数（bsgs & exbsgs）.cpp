namespace BSGS {
    int power(int a, int b, int m) {
        int r = 1 % m;
        a %= m;
        for (; b; b >>= 1, a = a * a % m)
            if (b & 1) r = r * a % m;
        return r;
    }

    int exgcd(int a, int b, int &x, int &y) {
        if (!b) {
            x = 1, y = 0;
            return a;
        }
        int d = exgcd(b, a % b, y, x);
        y -= a / b * x;
        return d;
    }
    int inv(int a, int m) {
        int x, y, d = exgcd(a, m, x, y);
        return d == 1 ? (x % m + m) % m : -1;
    }

    int bsgs(int a, int b, int p) {
        a %= p, b %= p;
        if (b == 1) return 0;

        int m = sqrtl(p) + 1, e = 1;
        map<int, int> mp;
        for (int j = 0; j < m; ++j) {
            mp.emplace(e, j);
            e = e * a % p;
        }

        int step = inv(power(a, m, p), p), cur = b;
        for (int i = 0; i <= m; ++i) {
            auto it = mp.find(cur);
            if (it != mp.end()) return i * m + it->second;
            cur = cur * step % p;
        }
        return -1;
    }

    int exbsgs(int a, int b, int p) {
        if (p == 1) return 0;
        a %= p, b %= p;
        if (a < 0) a += p;
        if (b < 0) b += p;
        if (b == 1) return 0;

        int k = 0, t = 1, g;
        while ((g = gcd(a, p)) > 1) {
            if (b % g) return -1;
            b /= g, p /= g, ++k;
            if (p == 1) return k;

            t = t * (a / g) % p % p;
            if (t == b) return k;
        }

        int it = inv(t, p);
        int x = bsgs(a, b * it % p, p);
        return x < 0 ? -1 : x + k;
    }
} // namespace BSGS

using namespace BSGS;
