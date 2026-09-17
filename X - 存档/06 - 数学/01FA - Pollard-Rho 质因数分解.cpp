/*
用途：分解 u64 范围的大整数。
依赖：01F - Miller-Rabin 素性检验。
复杂度：期望约 O(n^(1/4))；适合 n 很大、查询不多，不能替代小范围线性筛。
*/

using u64 = uint64_t;

namespace Pollard {

inline u64 add(u64 a, u64 b, u64 mod) {
    return a >= mod - b ? a - (mod - b) : a + b;
}

inline u64 next(u64 x, u64 c, u64 mod) {
    return add(Miller::mul(x, x, mod), c, mod);
}

inline u64 rand(u64 l, u64 r) {
    static mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    return uniform_int_distribution<u64>(l, r)(rng);
}

inline u64 find(u64 n) {
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;
    while (true) {
        u64 c = rand(1, n - 1);
        u64 x = rand(0, n - 1), y = x, d = 1;
        while (d == 1) {
            x = next(x, c, n);
            y = next(next(y, c, n), c, n);
            u64 dif = x >= y ? x - y : y - x;
            d = gcd(dif, n);
        }
        if (d != n) return d;
    }
}

inline void dfs(u64 n, vector<u64>& fac) {
    if (n == 1) return;
    if (Miller::ask(n)) {
        fac.push_back(n);
        return;
    }
    u64 d = find(n);
    dfs(d, fac);
    dfs(n / d, fac);
}

inline vector<u64> factor(u64 n) {
    assert(n >= 1);
    vector<u64> fac;
    dfs(n, fac);
    sort(fac.begin(), fac.end());
    return fac;
}
}  // namespace Pollard
