/*
用途：二维多项式卷积。返回 c[i][j] = sum a[x][y] b[i-x][j-y] (mod P)。
依赖：08 - 快速数论变换（NTT）。
用法：auto c = conv2<998244353, 3>(a,b)，a/b 均为非空矩形矩阵。
复杂度：O(NM(log N + log M))，N/M 为两维补齐后的 2 的幂。
前提：P 是原根 G 对应的 NTT 友好质数，且 N/M 分别整除 P-1。
*/

template <int P = 998244353, int G = 3>
vector<vector<int>> conv2(const vector<vector<int>>& a, const vector<vector<int>>& b) {
    assert(!a.empty() && !b.empty() && !a[0].empty() && !b[0].empty());
    int n1 = a.size(), m1 = a[0].size(), n2 = b.size(), m2 = b[0].size();
    for (const auto& row : a) assert(row.size() == a[0].size());
    for (const auto& row : b) assert(row.size() == b[0].size());
    int n = n1 + n2 - 1, m = m1 + m2 - 1;
    int nn = 1, mm = 1;
    while (nn < n) nn <<= 1;
    while (mm < m) mm <<= 1;

    vector<vector<int>> x(nn, vector<int>(mm)), y(nn, vector<int>(mm));
    for (int i = 0; i < n1; ++i) {
        for (int j = 0; j < m1; ++j) {
            x[i][j] = a[i][j] % P;
            if (x[i][j] < 0) x[i][j] += P;
        }
    }
    for (int i = 0; i < n2; ++i) {
        for (int j = 0; j < m2; ++j) {
            y[i][j] = b[i][j] % P;
            if (y[i][j] < 0) y[i][j] += P;
        }
    }

    auto ntt2 = [&](vector<vector<int>>& mat, bool iv) {
        for (int i = 0; i < nn; ++i) NTT<P, G>::ntt(mat[i], iv);
        vector<int> col(nn);
        for (int j = 0; j < mm; ++j) {
            for (int i = 0; i < nn; ++i) col[i] = mat[i][j];
            NTT<P, G>::ntt(col, iv);
            for (int i = 0; i < nn; ++i) mat[i][j] = col[i];
        }
    };
    ntt2(x, false);
    ntt2(y, false);
    for (int i = 0; i < nn; ++i) {
        for (int j = 0; j < mm; ++j) x[i][j] = x[i][j] * y[i][j] % P;
    }
    ntt2(x, true);
    x.resize(n);
    for (auto& v : x) v.resize(m);
    return x;
}
