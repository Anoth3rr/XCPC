void ntt2D(vector<vector<int>> &a, bool iv) {
    int n = a.size(), m = a[0].size();
    for (int i = 0; i < n; i++) ntt(a[i], iv);
    vector<int> t1(n);
    for (int j = 0; j < m; j++) {
        for (int i = 0; i < n; i++) t1[i] = a[i][j];
        ntt(t1, iv);
        for (int i = 0; i < n; i++) a[i][j] = t1[i];
    }
}
