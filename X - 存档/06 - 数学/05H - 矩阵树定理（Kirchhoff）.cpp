/*
用途：无向带权图生成树数。
依赖：05E - 行列式（模质数）；Z 通常为 MInt<P>。
*/
template <class Z> Z kirch(int n, const vector<tuple<int, int, Z>>& e) {
    assert(n >= 0);
    if (n <= 1) return Z(1);
    vector<vector<Z>> a(n, vector<Z>(n));
    for (auto [u, v, w] : e) {
        assert(0 <= u && u < n && 0 <= v && v < n);
        if (u == v) continue;
        a[u][u] += w;
        a[v][v] += w;
        a[u][v] -= w;
        a[v][u] -= w;
    }
    vector<vector<Z>> b(n - 1, vector<Z>(n - 1));
    for (int i = 0; i + 1 < n; ++i)
        for (int j = 0; j + 1 < n; ++j) b[i][j] = a[i][j];
    return det(move(b));
}
