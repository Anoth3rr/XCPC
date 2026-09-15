/*
用途：质数模域上的线性方程组，区分唯一解、无穷多解、无解。
依赖：02A - 自动取模（MInt）。
*/
enum class GaussStat { Unique, Infinite, Inconsistent };

template <class Z> struct GaussRes {
    GaussStat st;
    vector<Z> x;
    vector<int> p;
};

template <class Z> GaussRes<Z> gauss(vector<vector<Z>> a) {
    int n = a.size();
    if (!n) return {GaussStat::Unique, {}, {}};
    int m = a[0].size() - 1;
    assert(m >= 0);
    for (const auto& v : a) assert(v.size() == m + 1);

    vector<int> p;
    int r = 0;
    for (int c = 0; c < m && r < n; ++c) {
        int x = r;
        while (x < n && a[x][c] == Z(0)) ++x;
        if (x == n) continue;
        swap(a[x], a[r]);
        Z iv = a[r][c].inv();
        for (int j = c; j <= m; ++j) a[r][j] *= iv;
        for (int i = 0; i < n; ++i) {
            if (i == r || a[i][c] == Z(0)) continue;
            Z z = a[i][c];
            for (int j = c; j <= m; ++j) a[i][j] -= z * a[r][j];
        }
        p.push_back(c);
        ++r;
    }
    for (int i = r; i < n; ++i)
        if (a[i][m] != Z(0)) return {GaussStat::Inconsistent, {}, p};
    vector<Z> x(m);
    for (int i = 0; i < r; ++i) x[p[i]] = a[i][m];
    return {r == m ? GaussStat::Unique : GaussStat::Infinite, x, p};
}
