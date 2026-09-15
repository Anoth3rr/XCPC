/*
用途：求最短递推 a[t] = sum c[i] a[t-1-i]。
前提：Z 为域；返回的系数可直接交给 03KA。
*/
template <class Z> vector<Z> BM(const vector<Z>& a) {
    vector<Z> c{Z(1)}, b{Z(1)};
    int l = 0, m = 1;
    Z d = 1;
    for (int i = 0; i < a.size(); ++i) {
        Z x = a[i];
        for (int j = 1; j <= l; ++j) x += c[j] * a[i - j];
        if (x == Z(0)) {
            ++m;
            continue;
        }
        vector<Z> t = c;
        Z q = x / d;
        if (c.size() < b.size() + m) c.resize(b.size() + m);
        for (int j = 0; j < b.size(); ++j) c[j + m] -= q * b[j];
        if (l * 2 <= i) {
            l = i + 1 - l;
            b = move(t);
            d = x;
            m = 1;
        } else {
            ++m;
        }
    }
    vector<Z> r(l);
    for (int i = 0; i < l; ++i) r[i] = -c[i + 1];
    return r;
}
