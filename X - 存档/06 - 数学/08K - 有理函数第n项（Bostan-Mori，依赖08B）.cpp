/*
用途：求 [x^n] f(x)/g(x)。常用于由生成函数给出的超大下标递推。
依赖：02A - 自动取模（MInt）、08B - 多项式（Poly, Z）。
接口：divAt<Z,3>(f,g,n)。要求 g[0] != 0，返回 f/g 的第 n 项。
复杂度：O(d log d log n)，d 为 g 的次数量级；模数须为 NTT 友好质数。
*/

template <class Z, int G = 3> Z divAt(vector<Z> f, vector<Z> g, int n) {
    assert(n >= 0 && !g.empty() && g[0] != Z(0));
    while (n) {
        vector<Z> h = g;
        for (int i = 1; i < h.size(); i += 2) h[i] = -h[i];
        vector<Z> a = Poly<Z, G>::mul(f, h);
        vector<Z> b = Poly<Z, G>::mul(g, h);
        vector<Z> nf, ng;
        for (int i = n & 1; i < a.size(); i += 2) nf.push_back(a[i]);
        for (int i = 0; i < b.size(); i += 2) ng.push_back(b[i]);
        f = move(nf);
        g = move(ng);
        n >>= 1;
    }
    return f[0] / g[0];
}
