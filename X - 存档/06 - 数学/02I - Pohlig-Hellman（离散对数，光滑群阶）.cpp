/*
用途：Pohlig-Hellman 离散对数。p-1 平滑时，比通用 BSGS 更快。
依赖：02 - 快速幂（ModArith64）、02BA - 扩展欧几里得（inv）。
前提：p 为质数，g 是模 p 的原根；fac 是 p-1 的质因子分解 (质数, 指数)。
接口：solve(g,h,p,fac) 返回最小非负 x 使 g^x = h (mod p)；solve(g,h,p) 自动试除分解 p-1。
复杂度：O(sum sqrt(q) log p)，q 为 p-1 的不同质因子；适合 q 较小的光滑群阶。
*/

namespace Pohlig {
inline vector<pii> factor(int x) {
    vector<pii> res;
    for (int p = 2; p <= x / p; ++p) {
        if (x % p) continue;
        int e = 0;
        do x /= p, ++e; while (x % p == 0);
        res.push_back({p, e});
    }
    if (x > 1) res.push_back({x, 1});
    return res;
}

inline optional<int> askLog(int a, int b, int md, int ord) {
    int blk = sqrtl(ord);
    while (blk < ord / blk || (blk == ord / blk && blk * blk < ord)) ++blk;
    unordered_map<int, int> mp;
    mp.reserve(blk * 2 + 1);
    int cur = 1;
    for (int j = 0; j < blk; ++j) {
        if (!mp.count(cur)) mp.emplace(cur, j);
        cur = ModArith64::mul(cur, a, md);
    }
    int stp = ModArith64::power(a, (ord - blk % ord) % ord, md);
    cur = b;
    for (int i = 0; i <= blk; ++i) {
        auto it = mp.find(cur);
        if (it != mp.end()) {
            int x = i * blk + it->second;
            if (x < ord) return x;
        }
        cur = ModArith64::mul(cur, stp, md);
    }
    return nullopt;
}

inline optional<int> solve(int g, int h, int md, const vector<pii>& fac) {
    assert(md >= 2);
    g %= md;
    h %= md;
    if (g < 0) g += md;
    if (h < 0) h += md;
    if (!g || !h) return nullopt;
    if (md == 2) return h == 1 ? optional<int>(0) : nullopt;

    int ord = md - 1, all = 1;
    for (auto [q, e] : fac) {
        assert(q > 1 && e > 0);
        for (int i = 0; i < e; ++i) all *= q;
    }
    if (all != ord) return nullopt;

    int ans = 0, mul = 1;
    for (auto [q, e] : fac) {
        int cur = 0, pw = 1, qe = 1;
        int bas = ModArith64::power(g, ord / q, md);
        for (int i = 0; i < e; ++i) {
            int ig = ModArith64::power(g, (ord - cur % ord) % ord, md);
            int val = ModArith64::mul(h, ig, md);
            val = ModArith64::power(val, ord / (pw * q), md);
            auto dig = askLog(bas, val, md, q);
            if (!dig) return nullopt;
            cur += *dig * pw;
            pw *= q;
            qe *= q;
        }

        int dif = (cur - ans) % qe;
        if (dif < 0) dif += qe;
        auto iv = inv(mul % qe, qe);
        assert(iv.has_value());
        int k = ModArith64::mul(dif, *iv, qe);
        int nxt = mul * qe;
        ans = (ans + ModArith64::mul(mul, k, nxt)) % nxt;
        mul = nxt;
    }
    return ans;
}

inline optional<int> solve(int g, int h, int md) {
    return solve(g, h, md, factor(md - 1));
}
}  // namespace Pohlig
