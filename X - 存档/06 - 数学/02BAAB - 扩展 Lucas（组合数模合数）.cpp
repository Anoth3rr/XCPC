/*
用途：计算 C(n,k) mod m，m 可以是合数。
依赖：02 - ModArith64、02BA - inv、02BAA - 广义 CRT。
前提：每个质数幂 p^q 会预处理 O(p^q) 数组，适合 m 的各质因子幂可开（典型 m <= 2e6）；质数模优先用 03 / 03A。
*/

namespace ExLucas {
struct Comb {
    int pri, pk;
    int pe = 0;
    vector<int> fac;

    Comb(int p, int pk, int pe)
        : pri(p), pk(pk), pe(pe) {
        assert(pri >= 2 && pk >= pri && pk <= numeric_limits<int>::max());
        fac.assign(pk + 1, 1);
        for (int val = 1; val <= pk; ++val) {
            fac[val] = fac[val - 1];
            if (val % pri != 0) {
                fac[val] = ModArith64::mul(fac[val], val, pk);
            }
        }
    }

    int cntP(int val) const {
        int ans = 0;
        while (val > 0) val /= pri, ans += val;
        return ans;
    }

    int facP(int val) const {
        if (val == 0) return 1;
        int ans = ModArith64::power(fac[pk], val / pk, pk);
        ans = ModArith64::mul(ans, fac[val % pk], pk);
        return ModArith64::mul(ans, facP(val / pri), pk);
    }

    int C(int n, int k) const {
        if (k < 0 || k > n) return 0;
        int e = cntP(n) - cntP(k) - cntP(n - k);
        if (e >= pe) return 0;

        int a = facP(n);
        int bl = facP(k);
        int br = facP(n - k);
        auto il = inv(bl, pk);
        auto ir = inv(br, pk);
        assert(il && ir);
        int ans = ModArith64::mul(a, *il, pk);
        ans = ModArith64::mul(ans, *ir, pk);
        ans = ModArith64::mul(ans, ModArith64::power(pri, e, pk), pk);
        return ans;
    }
};

inline vector<tuple<int, int, int>> factor(int mod) {
    assert(mod >= 1);
    vector<tuple<int, int, int>> fac;
    for (int d = 2; d <= mod / d; ++d) {
        if (mod % d != 0) continue;
        int pk = 1;
        int e = 0;
        do {
            mod /= d;
            pk *= d;
            ++e;
        } while (mod % d == 0);
        fac.emplace_back(d, pk, e);
    }
    if (mod > 1) fac.emplace_back(mod, mod, 1);
    return fac;
}

inline int C(int n, int k, int mod) {
    assert(n >= 0 && mod > 0);
    if (k < 0 || k > n || mod == 1) return 0;
    vector<CRTResult> eq;
    for (auto [pri, pk, e] : factor(mod)) {
        Comb cal(pri, pk, e);
        eq.push_back({cal.C(n, k), pk});
    }
    auto res = crt(eq);
    assert(res && res->mod == mod);
    return res->r;
}
}  // namespace ExLucas
