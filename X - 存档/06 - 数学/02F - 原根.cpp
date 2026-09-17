/*
用途：判断模数是否有原根，寻找最小/全部原根，并判断给定数是否为原根。
前提：依赖 02 - 快速幂（ModArith64）。原根仅存在于 2、4、p^k、2p^k（p 为奇质数）。
接口：exist(n) 判断是否存在；solve(n) 返回最小原根，不存在返回 -1；roots(n) 返回递增的全部原根；ask(g, n) 判断 g。
复杂度：试除分解 O(sqrt n)，单次验证 O(ω(phi(n)) log n)；枚举全部原根 O(phi(n) log n)。
*/

namespace PrimRoot {
inline vector<int> factor(int val) {
    assert(val >= 1);
    vector<int> fac;
    for (int d = 2; d <= val / d; ++d) {
        if (val % d != 0) continue;
        fac.push_back(d);
        do val /= d; while (val % d == 0);
    }
    if (val > 1) fac.push_back(val);
    return fac;
}

inline bool exist(int mod) {
    assert(mod >= 2);
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
    for (int p : factor(mod)) res = res / p * (p - 1);
    return res;
}

inline bool ask(int g, int mod, int ph, const vector<int>& fac) {
    g %= mod;
    if (g <= 0) g += mod;
    if (gcd(g, mod) != 1) return false;
    for (int p : fac) {
        if (ModArith64::power(g, ph / p, mod) == 1) return false;
    }
    return true;
}

inline bool ask(int g, int mod) {
    assert(mod >= 2);
    if (!exist(mod)) return false;
    int ph = phi(mod);
    return ask(g, mod, ph, factor(ph));
}

inline int find(int mod, int ph, const vector<int>& fac) {
    for (int g = 1; g < mod; ++g) {
        if (ask(g, mod, ph, fac)) return g;
    }
    return -1;
}

inline int solve(int mod) {
    assert(mod >= 2);
    if (!exist(mod)) return -1;
    int ph = phi(mod);
    return find(mod, ph, factor(ph));
}

inline vector<int> roots(int mod) {
    assert(mod >= 2);
    if (!exist(mod)) return {};
    int ph = phi(mod);
    vector<int> fac = factor(ph);
    int g = find(mod, ph, fac), cnt = phi(ph);
    vector<int> res;
    res.reserve(cnt);
    for (int k = 1, x = g; k <= ph; ++k) {
        if (gcd(k, ph) == 1) res.push_back(x);
        x = ModArith64::mul(x, g, mod);
    }
    sort(res.begin(), res.end());
    return res;
}
}  // namespace PrimRoot
