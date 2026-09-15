/*
用途：求 f(x)^e 的前 n 项，e 为非负十进制整数串。
依赖：02A - 自动取模（MInt）、08B - 多项式（Poly, Z）。接口：ppow<Z,3>(f, e, n)。
限制：模数为 NTT 友好质数且 n < mod；e 只能含十进制数字。
区别：08N 的 ppow 接收普通整数指数；本分支保留超大指数环境。
*/

template <class Z, int G = 3> vector<Z> ppow(vector<Z> f, string e, int n) {
    assert(n >= 0);
    if (!n) return {};
    assert(n < Z::askMod());
    assert(!e.empty());
    for (char c : e) assert('0' <= c && c <= '9');
    auto q = e.find_first_not_of('0');
    e = q == string::npos ? "0" : e.substr(q);
    if (e == "0") {
        vector<Z> r(n);
        r[0] = Z(1);
        return r;
    }

    int p = 0;
    f.resize(n);
    while (p < n && f[p] == Z(0)) ++p;
    if (p == n) return vector<Z>(n);

    int lim = p ? (n - 1) / p : 0, v = 0;
    bool big = false;
    if (p) {
        for (char c : e) {
            int d = c - '0';
            if (v > lim / 10 || (v == lim / 10 && d > lim % 10)) {
                big = true;
                break;
            }
            v = v * 10 + d;
        }
    }
    if (big) return vector<Z>(n);

    int md = Z::askMod(), a = 0, b = 0;
    for (char c : e) {
        a = (a * 10 + c - '0') % md;
        b = (b * 10 + c - '0') % (md - 1);
    }
    int sh = p * v, m = n - sh;
    Z c = f[p];
    vector<Z> x(f.begin() + p, f.end());
    x.resize(m);
    for (Z& z : x) z /= c;
    x = Poly<Z, G>::ln(x, m);
    for (Z& z : x) z *= Z(a);
    x = Poly<Z, G>::exp(x, m);
    c = c.pow(b);

    vector<Z> r(n);
    for (int i = 0; i < m; ++i) r[i + sh] = x[i] * c;
    return r;
}
