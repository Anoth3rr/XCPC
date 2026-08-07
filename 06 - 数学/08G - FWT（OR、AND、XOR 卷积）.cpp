/*
用途：长度 2^k 的 OR / AND / XOR 卷积。
前提：XOR 逆变换要求 2 在 Z 中可逆。
*/
enum class FWTType { Or, And, Xor };

template <class Z> void fwt(vector<Z>& a, FWTType tp, bool iv) {
    int n = a.size();
    assert(n && !(n & (n - 1)));
    for (int l = 1; l < n; l <<= 1) {
        for (int i = 0; i < n; i += l << 1) {
            for (int j = 0; j < l; ++j) {
                Z& x = a[i + j];
                Z& y = a[i + j + l];
                if (tp == FWTType::Or) {
                    if (iv) y -= x;
                    else y += x;
                } else if (tp == FWTType::And) {
                    if (iv) x -= y;
                    else x += y;
                } else {
                    Z u = x, v = y;
                    x = u + v;
                    y = u - v;
                }
            }
        }
    }
    if (tp == FWTType::Xor && iv) {
        Z x = Z(n).inv();
        for (Z& v : a) v *= x;
    }
}

template <class Z> vector<Z> conv(vector<Z> a, vector<Z> b, FWTType tp) {
    assert(a.size() == b.size() && !a.empty());
    fwt(a, tp, false);
    fwt(b, tp, false);
    for (int i = 0; i < a.size(); ++i) a[i] *= b[i];
    fwt(a, tp, true);
    return a;
}
