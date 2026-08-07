/*
用途：求 1..n 排列的康托展开排名模 mod。
复杂度：O(n log n)。结果只保留模 mod；n <= 20 且需要精确排名或反排名时使用 03B。
*/

using i128 = __int128_t;

struct Cantor {
    static int mul(int a, int b, int mod) {
        assert(mod > 0);
        a %= mod;
        b %= mod;
#ifdef __SIZEOF_INT128__
        return i128(a) * b % mod;
#else
        int res = 0;
        while (b > 0) {
            if (b & 1) res = res >= mod - a ? res - (mod - a) : res + a;
            a = a >= mod - a ? a - (mod - a) : a + a;
            b >>= 1;
        }
        return res;
#endif
    }

    static void check(const vector<int>& p) {
        int n = p.size();
        vector<char> vis(n + 1, false);
        for (int val : p) {
            assert(1 <= val && val <= n && !vis[val]);
            vis[val] = true;
        }
    }

    static int askMod(const vector<int>& p, int mod) {
        assert(mod > 0);
        int n = p.size();
        check(p);
        struct BIT {
            int n;
            vector<int> t;
            explicit BIT(int n) : n(n), t(n + 1, 0) {}
            void modify(int x, int v) { for (; x <= n; x += x & -x) t[x] += v; }
            int ask(int x) const { int r = 0; for (; x > 0; x -= x & -x) r += t[x]; return r; }
        } bit(n);

        vector<int> fac(n + 1, 1 % mod);
        for (int i = 1; i <= n; ++i) fac[i] = mul(fac[i - 1], i, mod);
        for (int x = 1; x <= n; ++x) bit.modify(x, 1);

        int rk = 1 % mod;
        for (int i = 0; i < n; ++i) {
            int cnt = bit.ask(p[i] - 1);
            int get = mul(cnt, fac[n - 1 - i], mod);
            rk = rk >= mod - get ? rk - (mod - get) : rk + get;
            bit.modify(p[i], -1);
        }
        return rk;
    }

    // 质数模时优先使用本重载，Z 自动完成加乘取模。
    template <class Z> static Z askRank(const vector<int>& p) {
        int n = p.size();
        check(p);
        struct BIT {
            int n;
            vector<int> t;
            explicit BIT(int n) : n(n), t(n + 1) {}
            void modify(int x, int v) {
                for (; x <= n; x += x & -x) t[x] += v;
            }
            int ask(int x) const {
                int r = 0;
                for (; x; x -= x & -x) r += t[x];
                return r;
            }
        } b(n);
        vector<Z> f(n + 1, Z(1));
        for (int i = 1; i <= n; ++i) f[i] = f[i - 1] * i;
        for (int x = 1; x <= n; ++x) b.modify(x, 1);
        Z r = 1;
        for (int i = 0; i < n; ++i) {
            r += Z(b.ask(p[i] - 1)) * f[n - 1 - i];
            b.modify(p[i], -1);
        }
        return r;
    }
};
