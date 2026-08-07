/*
用途：求 max c·x，满足 A·x <= b、x >= 0 的浮点线性规划。
返回最优、无界或无解；使用两阶段单纯形法。浮点数据请按量级调整 EPS。
复杂度：单纯形最坏指数，竞赛中的中小规模通常可用；不能处理整数规划。
*/
using ld = long double;

enum class LPStatus { Optimal, Infeasible, Unbounded };

struct LPRes {
    LPStatus st;
    ld ans = 0;
    vector<ld> sol;
};

struct Simplex {
    static constexpr ld EPS = 1e-12L;
    int n, m;
    vector<int> bas, nb;
    vector<vector<ld>> a;

    Simplex(const vector<vector<ld>>& A, const vector<ld>& b,
                             const vector<ld>& c)
        : n(b.size()), m(c.size()), bas(n),
          nb(m + 1), a(n + 2, vector<ld>(m + 2, 0)) {
        assert(A.size() == n);
        for (int i = 0; i < n; ++i) {
            assert(A[i].size() == m);
            for (int j = 0; j < m; ++j) a[i][j] = A[i][j];
        }
        for (int i = 0; i < n; ++i) {
            bas[i] = m + i;
            a[i][m] = -1;
            a[i][m + 1] = b[i];
        }
        for (int j = 0; j < m; ++j) {
            nb[j] = j;
            a[n][j] = -c[j];
        }
        nb[m] = -1;
        a[n + 1][m] = 1;
    }

    void pivot(int row, int c) {
        ld inv = 1 / a[row][c];
        for (int i = 0; i < n + 2; ++i) {
            if (i == row) continue;
            for (int j = 0; j < m + 2; ++j) {
                if (j == c) continue;
                a[i][j] -= a[row][j] * a[i][c] * inv;
            }
        }
        for (int j = 0; j < m + 2; ++j) if (j != c) a[row][j] *= inv;
        for (int i = 0; i < n + 2; ++i) if (i != row) a[i][c] *= -inv;
        a[row][c] = inv;
        swap(bas[row], nb[c]);
    }

    bool simplex(int ph) {
        int obj = ph == 1 ? n + 1 : n;
        while (true) {
            int in = -1;
            for (int c = 0; c <= m; ++c) {
                if (ph == 2 && nb[c] == -1) continue;
                if (in == -1 || a[obj][c] < a[obj][in] - EPS ||
                    (fabsl(a[obj][c] - a[obj][in]) <= EPS &&
                     nb[c] < nb[in])) {
                    in = c;
                }
            }
            if (a[obj][in] >= -EPS) return true;

            int out = -1;
            for (int row = 0; row < n; ++row) {
                if (a[row][in] <= EPS) continue;
                if (out == -1 ||
                    a[row][m + 1] / a[row][in] <
                        a[out][m + 1] / a[out][in] - EPS ||
                    (fabsl(a[row][m + 1] / a[row][in] -
                           a[out][m + 1] / a[out][in]) <= EPS &&
                     bas[row] < bas[out])) {
                    out = row;
                }
            }
            if (out == -1) return false;
            pivot(out, in);
        }
    }

    LPRes solve() {
        if (n == 0) {
            for (int j = 0; j < m; ++j)
                if (a[n][j] < -EPS) return {LPStatus::Unbounded, 0, {}};
            return {LPStatus::Optimal, 0, vector<ld>(m, 0)};
        }

        int row = 0;
        for (int i = 1; i < n; ++i)
            if (a[i][m + 1] < a[row][m + 1]) row = i;
        if (a[row][m + 1] < -EPS) {
            pivot(row, m);
            if (!simplex(1) || a[n + 1][m + 1] < -EPS) {
                return {LPStatus::Infeasible, 0, {}};
            }
            if (fabsl(a[n + 1][m + 1]) > EPS) {
                return {LPStatus::Infeasible, 0, {}};
            }
            for (int i = 0; i < n; ++i) {
                if (bas[i] != -1) continue;
                int c = 0;
                for (int j = 1; j <= m; ++j) {
                    if (a[i][j] < a[i][c] - EPS ||
                        (fabsl(a[i][j] - a[i][c]) <= EPS && nb[j] < nb[c])) {
                        c = j;
                    }
                }
                pivot(i, c);
            }
        }
        if (!simplex(2)) return {LPStatus::Unbounded, 0, {}};
        vector<ld> sol(m, 0);
        for (int i = 0; i < n; ++i) {
            if (bas[i] < m) sol[bas[i]] = a[i][m + 1];
        }
        return {LPStatus::Optimal, a[n][m + 1], sol};
    }
};
