/*
用途：广义中国剩余定理，合并 x ≡ residue (mod modulus)；模数不要求互质。
依赖：02 - 快速幂（ModArith64）与 02BA - 扩展欧几里得（inv）。
返回 nullopt：方程无解，或合并后的 lcm 超出 int。
*/

struct CRTResult {
    int r;  // 始终规范到 [0, modulus)。
    int mod;
};

inline int norm(int val, int mod) {
    val %= mod;
    return val < 0 ? val + mod : val;
}

inline optional<CRTResult> merge(CRTResult x, CRTResult y) {
    assert(x.mod > 0 && y.mod > 0);
    x.r = norm(x.r, x.mod);
    y.r = norm(y.r, y.mod);

    int g = gcd(x.mod, y.mod);
    int dif = y.r - x.r;
    if (dif % g != 0) return nullopt;

    int b = y.mod / g;
    if (x.mod > numeric_limits<int>::max() / b) return nullopt;
    int lcm = x.mod * b;

    int k = 0;
    if (b != 1) {
        auto iv = inv(x.mod / g, b);
        assert(iv.has_value());
        int rhs = norm(dif / g, b);
        k = ModArith64::mul(rhs, *iv, b);
    }

    int add = ModArith64::mul(x.mod, k, lcm);
    int r = x.r >= lcm - add
                            ? x.r - (lcm - add)
                            : x.r + add;
    return CRTResult{r, lcm};
}

inline optional<CRTResult> crt(const vector<CRTResult>& eq) {
    if (eq.empty()) return CRTResult{0, 1};
    CRTResult res = eq[0];
    if (res.mod <= 0) return nullopt;
    res.r = norm(res.r, res.mod);
    for (int i = 1; i < eq.size(); ++i) {
        if (eq[i].mod <= 0) return nullopt;
        auto z = merge(res, eq[i]);
        if (!z) return nullopt;
        res = *z;
    }
    return res;
}
