/*
用途：域上的方阵行列式；MInt 时模数须为质数。
*/
template <class Z> Z det(vector<vector<Z>> a) {
    int n = a.size();
    for (const auto& v : a) assert(v.size() == n);
    Z r = 1;
    for (int c = 0; c < n; ++c) {
        int p = c;
        while (p < n && a[p][c] == Z(0)) ++p;
        if (p == n) return Z(0);
        if (p != c) {
            swap(a[p], a[c]);
            r = -r;
        }
        Z x = a[c][c];
        r *= x;
        Z iv = x.inv();
        for (int i = c + 1; i < n; ++i) {
            if (a[i][c] == Z(0)) continue;
            Z q = a[i][c] * iv;
            for (int j = c; j < n; ++j) a[i][j] -= q * a[c][j];
        }
    }
    return r;
}
