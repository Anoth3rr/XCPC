/*
用途：在 GF(2) 上求异或线性方程组。每行 bitset 的 [0,variables) 为系数，variables 位为右端常数。
前提：variables <= MAX_VARIABLES；加减与乘法均为 xor/and。
复杂度：O(rows * variables * MAX_VARIABLES / word_bits)，适合布尔方程、异或约束。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
enum class GaussStat { Unique, Infinite, Inconsistent };

template <int M> struct GaussRes {
    GaussStat st;
    vector<int> sol;  // Infinite 时自由元取 0 的一组特解。
    vector<int> pc;
};

template <int M>
GaussRes<M> gauss(vector<bitset<M + 1>> a,
                                                            int m) {
    assert(0 <= m && m <= M);
    int n = a.size();
    vector<int> pc;
    int pr = 0;
    for (int c = 0; c < m && pr < n; ++c) {
        int vis = pr;
        while (vis < n && !a[vis][c]) ++vis;
        if (vis == n) continue;
        swap(a[vis], a[pr]);
        for (int row = 0; row < n; ++row) {
            if (row != pr && a[row][c]) a[row] ^= a[pr];
        }
        pc.push_back(c);
        ++pr;
    }
    for (int row = pr; row < n; ++row) {
        bool has = false;
        for (int c = 0; c < m; ++c) has = has || a[row][c];
        if (!has && a[row][m]) return {GaussStat::Inconsistent, {}, pc};
    }
    vector<int> sol(m, 0);
    for (int row = 0; row < pr; ++row) sol[pc[row]] = a[row][m];
    return {pr == m ? GaussStat::Unique : GaussStat::Infinite, sol, pc};
}
