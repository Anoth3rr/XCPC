/*
裴蜀定理：整数 a_1...a_k 的所有线性组合恰为 gcd(a_1,...,a_k) 的倍数。
用途：判断方程 sum a_i * x_i = target 是否有整数解。
*/

inline int gcdAll(const vector<int>& a) {
    int res = 0;
    for (int val : a) res = gcd(res, val);
    return res >= 0 ? res : -res;
}

inline bool ask(const vector<int>& coe, int t) {
    int d = gcdAll(coe);
    return d == 0 ? t == 0 : t % d == 0;
}
