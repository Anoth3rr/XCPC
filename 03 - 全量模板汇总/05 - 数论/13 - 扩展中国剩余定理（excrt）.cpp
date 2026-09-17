namespace excrt {
    constexpr int N = 1e5;
    int ai[N + 1], bi[N + 1];
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
    int excrt() {
        int x, y, k;
        int M = bi[1], ans = ai[1];
        for (int i = 2; i <= n; ++i) {
            int a = M, b = bi[i], c = (ai[i] - ans % b + b) % b;
            int gcd = exgcd(a, b, x, y), bg = b / gcd;
            if (c % gcd != 0) return -1;
            x = mul(x, c / gcd, bg);
            ans += x * M;
            M *= bg;
            ans = (ans % M + M) % M;
        }
        return (ans % M + M) % M;
    }
}; // namespace excrt

using namespace excrt;