namespace BSGS {
    map<int, int> f;
    inline int power(int a, int b, int m) {
        int r = 1 % m;
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
    // int inv(int a, int m) {
    // return power(a, m - 2, m);
    // }
    int inv(int a, int m) {
        int x, y, d = exgcd(a, m, x, y);
        return d == 1 ? (x % m + m) % m : -1;
    }
    int bsgs(int a, int b, int p) {
        f.clear();
        int m = ceil(sqrt(p));
        b %= p;
        for (int i = 1; i <= m; i++) {
            b = b * a % p;
            f[b] = i;
        }
        int tmp = power(a, m, p);
        b = 1;
        for (int i = 1; i <= m; i++) {
            b = b * tmp % p;
            if (f.find(b) != f.end()) return i * m - f[b];
        }
        return -1;
    }
    int exbsgs(int a, int b, int p) {
        if (b == 1 || p == 1) return 0;
        int g = gcd(a, p), k = 0, na = 1;
        while (g > 1) {
            if (b % g != 0) return -1;
            k++;
            b /= g;
            p /= g;
            na = na * (a / g) % p;
            if (na == b) return k;
            g = gcd(a, p);
        }
        int f = bsgs(a, b * inv(na, p) % p, p);
        if (f == -1) return -1;
        return f + k;
    }
} // namespace BSGS

using namespace BSGS;