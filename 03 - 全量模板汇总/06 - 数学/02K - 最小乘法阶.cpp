/*
用途：求互素元素 a 在模 m 下的最小乘法阶 ord_m(a)。
依赖：02 - 快速幂（ModArith64）。
前提：m > 1 且 gcd(a,m)=1；k 必须是 ord_m(a) 的倍数，通常取 phi(m) 或 Carmichael 函数 lambda(m)。
接口：MulOrd::ask(a,m) 自动试除 phi(m)；MulOrd::ask(a,m,k,fac) 使用已知群阶和其不同质因子，适合大数/质数模。
复杂度：自动版试除 O(sqrt m + sqrt phi(m))；约除阶段 O(omega(k) log k log m)。
*/

namespace MulOrd {
inline vector<int> factor(int x) {
    assert(x >= 1);
    vector<int> f;
    for (int p = 2; p <= x / p; ++p) {
        if (x % p) continue;
        f.push_back(p);
        do x /= p; while (x % p == 0);
    }
    if (x > 1) f.push_back(x);
    return f;
}

inline int phi(int m) {
    assert(m >= 1);
    int r = m;
    for (int p : factor(m)) r = r / p * (p - 1);
    return r;
}

inline int ask(int a, int m, int k, const vector<int> &fac) {
    assert(m > 1 && k > 0 && gcd(a, m) == 1);
    a %= m;
    if (a < 0) a += m;
    for (int p : fac) {
        assert(p > 1);
        while (k % p == 0 && ModArith64::power(a, k / p, m) == 1) k /= p;
    }
    return k;
}

inline int ask(int a, int m, int k) {
    return ask(a, m, k, factor(k));
}

inline int ask(int a, int m) {
    int k = phi(m);
    return ask(a, m, k);
}
}  // namespace MulOrd
