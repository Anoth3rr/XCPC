/*
用途：已知递推系数 c 和前 k 项 a，在 O(k^2 log n) 求第 n 项。
*/

namespace LinearRec {
template <class Z> vector<Z> combine(const vector<Z>& a, const vector<Z>& b, const vector<Z>& c) {
    int k = c.size();
    vector<Z> t(k * 2 - 1);
    for (int i = 0; i < k; ++i)
        for (int j = 0; j < k; ++j) t[i + j] += a[i] * b[j];
    for (int i = k * 2 - 2; i >= k; --i)
        for (int j = 0; j < k; ++j) t[i - 1 - j] += t[i] * c[j];
    t.resize(k);
    return t;
}

template <class Z> Z ask(const vector<Z>& a, const vector<Z>& c, int n) {
    int k = c.size();
    assert(k && a.size() == k && n >= 0);
    if (n < k) return a[n];
    vector<Z> r(k), x(k);
    r[0] = 1;
    if (k == 1) x[0] = c[0];
    else x[1] = 1;
    while (n) {
        if (n & 1) r = combine(r, x, c);
        x = combine(x, x, c);
        n >>= 1;
    }
    Z ans = 0;
    for (int i = 0; i < k; ++i) ans += r[i] * a[i];
    return ans;
}
}  // namespace LinearRec
