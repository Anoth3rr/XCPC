/*
用途：计算 π(n)，即不超过 n 的质数个数。
复杂度：Lehmer prime counting，单次通常远快于开到 n 的筛；默认预处理到 5e6，适合 n 约 1e13 及以内。
前提：若 n 更大，init(limit) 的 limit 至少应覆盖 sqrt(n)；大量小范围查询直接使用 01 - 线性筛。
*/

using i128 = __int128_t;

using ld = long double;

struct Lehmer {
    int lim = 0;
    vector<int> pri, pc;
    unordered_map<int, int> mp;

    explicit Lehmer(int mx = 5'000'000) {
        init(mx);
    }

    void init(int mx) {
        assert(mx >= 2);
        lim = mx;
        vector<bool> vis(mx + 1, false);
        pc.assign(mx + 1, 0);
        pri.clear();
        for (int x = 2; x <= mx; ++x) {
            if (!vis[x]) pri.push_back(x);
            for (int p : pri) {
                if (p > mx / x) break;
                vis[p * x] = true;
                if (x % p == 0) break;
            }
        }
        for (int x = 1; x <= mx; ++x) pc[x] = pc[x - 1] + (!vis[x] && x >= 2);
        mp.clear();
    }

    static int isqrt(int val) {
        assert(val >= 0);
        int rt = sqrtl(ld(val));
        while (i128(rt + 1) * (rt + 1) <= val) ++rt;
        while (i128(rt) * rt > val) --rt;
        return rt;
    }

    static int icbrt(int val) {
        assert(val >= 0);
        int rt = cbrtl(ld(val));
        while (i128(rt + 1) * (rt + 1) * (rt + 1) <= val) ++rt;
        while (i128(rt) * rt * rt > val) --rt;
        return rt;
    }

    int phi(int val, int pi) const {
        if (pi == 0) return val;
        if (pi == 1) return val - val / 2;
        if (pi < pri.size() &&
            pri[pi - 1] * pri[pi - 1] > val) {
            assert(val <= lim);
            return pc[val] - pi + 1;
        }
        return phi(val, pi - 1) - phi(val / pri[pi - 1], pi - 1);
    }

    int ask(int val) {
        assert(val >= 0);
        if (val <= lim) return pc[val];
        if (auto it = mp.find(val); it != mp.end()) return it->second;

        int a = ask(isqrt(isqrt(val)));
        int b = ask(isqrt(val));
        int c = ask(icbrt(val));
        assert(b <= pri.size());
        int ans = phi(val, a) + (b + a - 2) * (b - a + 1) / 2;
        for (int i = a; i < b; ++i) {
            int q = val / pri[i];
            ans -= ask(q);
            if (i < c) {
                int hi = ask(isqrt(q));
                for (int j = i; j < hi; ++j) ans -= ask(q / pri[j]) - j;
            }
        }
        return mp[val] = ans;
    }
};
