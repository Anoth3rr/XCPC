/*
用途：给定互异横坐标点 (x[i], y[i])，在 p 处求值。
前提：Z 为质数模，横坐标两两不同。
*/
template <class Z> Z lagr(const vector<Z>& x, const vector<Z>& y, Z p) {
    int n = x.size();
    assert(n && y.size() == n);
    for (int i = 0; i < n; ++i) {
        if (p == x[i]) return y[i];
        for (int j = 0; j < i; ++j) assert(x[i] != x[j]);
    }
    Z r = 0;
    for (int i = 0; i < n; ++i) {
        Z a = 1, b = 1;
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            a *= p - x[j];
            b *= x[i] - x[j];
        }
        r += y[i] * a / b;
    }
    return r;
}
