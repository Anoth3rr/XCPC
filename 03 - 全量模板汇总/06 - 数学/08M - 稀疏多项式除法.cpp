/*
用途：计算 g(x)/f(x) 的前 n 项，其中 f 稀疏且 f[0] != 0。
依赖：02A - 自动取模（MInt）。
接口：spDiv(g,f)，返回与 g 等长的截断商；g/f 均按低次到高次存储。
复杂度：O(nk)，k 为 f 的非零项数。f 很稀疏时比 NTT 求逆更短、更快。
*/

template <class Z> vector<Z> spDiv(vector<Z> g, const vector<Z>& f) {
    assert(!f.empty() && f[0] != Z(0));
    vector<pair<int, Z>> nz;
    for (int i = 1; i < f.size(); ++i) {
        if (f[i] != Z(0)) nz.push_back({i, f[i]});
    }
    Z iv = f[0].inv();
    for (int i = 0; i < g.size(); ++i) {
        for (auto [j, x] : nz) {
            if (j > i) break;
            g[i] -= x * g[i - j];
        }
        g[i] *= iv;
    }
    return g;
}
