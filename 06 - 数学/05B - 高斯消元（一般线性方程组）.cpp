/*
用途：求一般实系数 m 元线性方程组，区分唯一解、无穷多解、无解。
复杂度：O(m^2 * rows)。浮点题请按数据范围调整 eps；精确模意义方程不能直接套用。
与 05C 都用于线性方程组，但 05C 只适合对称、无零主元的特殊矩阵。
*/
using ld = long double;

enum class GaussStat { Unique, Infinite, Inconsistent };

struct GaussRes {
    GaussStat st;
    vector<ld> sol;  // Infinite 时给出一个自由元均为 0 的特解。
    vector<int> pc;
};

inline GaussRes gauss(vector<vector<ld>> a, ld eps = 1e-12L) {
    int n = a.size();
    if (n == 0) return {GaussStat::Unique, {}, {}};
    int m = a[0].size() - 1;
    assert(m >= 0);
    for (const auto& row : a) assert(row.size() == m + 1);

    vector<int> pc;
    int pr = 0;
    for (int c = 0; c < m && pr < n; ++c) {
        int vis = pr;
        for (int row = pr + 1; row < n; ++row) {
            if (fabsl(a[row][c]) > fabsl(a[vis][c])) vis = row;
        }
        if (fabsl(a[vis][c]) <= eps) continue;
        swap(a[vis], a[pr]);
        ld iv = 1 / a[pr][c];
        for (int j = c; j <= m; ++j) a[pr][j] *= iv;
        for (int row = 0; row < n; ++row) {
            if (row == pr || fabsl(a[row][c]) <= eps) continue;
            ld coe = a[row][c];
            for (int j = c; j <= m; ++j) a[row][j] -= coe * a[pr][j];
        }
        pc.push_back(c);
        ++pr;
    }

    for (int row = pr; row < n; ++row) {
        if (fabsl(a[row][m]) > eps) return {GaussStat::Inconsistent, {}, pc};
    }
    vector<ld> sol(m, 0);
    for (int row = 0; row < pr; ++row) sol[pc[row]] = a[row][m];
    return {pr == m ? GaussStat::Unique : GaussStat::Infinite, sol, pc};
}
