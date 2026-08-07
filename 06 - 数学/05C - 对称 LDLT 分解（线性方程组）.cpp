/*
用途：以 LDL^T 分解求对称线性方程组。
复杂度：O(n^3)。不做主元交换，要求各顺序主子式不出现零枢轴；否则使用 05B。
与 05B 是同一问题的不同实现：本模板常数和内存更小，但适用条件更强。
*/
using ld = long double;

inline optional<vector<ld>> solve(
    const vector<vector<ld>>& mat, const vector<ld>& rhs, ld eps = 1e-12L) {
    int n = mat.size();
    if (rhs.size() != n) return nullopt;
    for (const auto& row : mat) if (row.size() != n) return nullopt;

    vector<vector<ld>> lo(n, vector<ld>(n, 0));
    vector<ld> dia(n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            ld val = mat[i][j];
            for (int k = 0; k < j; ++k) val -= lo[i][k] * dia[k] * lo[j][k];
            if (fabsl(dia[j]) <= eps) return nullopt;
            lo[i][j] = val / dia[j];
        }
        ld val = mat[i][i];
        for (int k = 0; k < i; ++k) val -= lo[i][k] * lo[i][k] * dia[k];
        if (fabsl(val) <= eps) return nullopt;
        dia[i] = val;
        lo[i][i] = 1;
    }

    vector<ld> y(n), z(n), ans(n);
    for (int i = 0; i < n; ++i) {
        y[i] = rhs[i];
        for (int j = 0; j < i; ++j) y[i] -= lo[i][j] * y[j];
        z[i] = y[i] / dia[i];
    }
    for (int i = n - 1; i >= 0; --i) {
        ans[i] = z[i];
        for (int j = i + 1; j < n; ++j) ans[i] -= lo[j][i] * ans[j];
    }
    return ans;
}
