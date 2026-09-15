/*
用途：有向正权图的最小简单环长度与计数。
约定：点编号 1..n；add 可加入重边，同权重边视为不同边；自环视为一个环。
接口：ask 返回 {len,cnt}；无环时 len=INF,cnt=0。
前提：边权严格为正；Z 用于计数，通常传 MInt。
复杂度：O(n^3) 时间、O(n^2) 空间。
*/
template <class Z>
struct MinCycCnt {
    static constexpr int INF = numeric_limits<int>::max() / 4;

    struct Res {
        int len;
        Z cnt;
    };

    int n, sl = INF;
    Z sc{};
    vector<vector<int>> a;
    vector<vector<Z>> b;

    explicit MinCycCnt(int n) : n(n), a(n + 1, vector<int>(n + 1, INF)), b(n + 1, vector<Z>(n + 1)) {}

    void add(int u, int v, int w) {
        assert(1 <= u && u <= n && 1 <= v && v <= n && 0 < w && w < INF);
        if (u == v) {
            if (w < sl) sl = w, sc = Z(1);
            else if (w == sl) sc += Z(1);
            return;
        }
        if (w < a[u][v]) a[u][v] = w, b[u][v] = Z(1);
        else if (w == a[u][v]) b[u][v] += Z(1);
    }

    Res ask() const {
        auto d = a;
        auto c = b;
        for (int i = 1; i <= n; ++i) d[i][i] = 0, c[i][i] = Z(1);
        int ans = sl;
        Z cnt = sc;
        auto upd = [&](int len, Z now) {
            if (len < ans) ans = len, cnt = now;
            else if (len == ans) cnt += now;
        };

        for (int k = 1; k <= n; ++k) {
            for (int i = 1; i < k; ++i) {
                if (a[k][i] != INF && d[i][k] != INF) upd(a[k][i] + d[i][k], b[k][i] * c[i][k]);
            }
            for (int i = 1; i <= n; ++i) {
                if (i == k || d[i][k] == INF) continue;
                for (int j = 1; j <= n; ++j) {
                    if (j == k || d[k][j] == INF) continue;
                    int nd = d[i][k] + d[k][j];
                    if (nd < d[i][j]) d[i][j] = nd, c[i][j] = c[i][k] * c[k][j];
                    else if (nd == d[i][j]) c[i][j] += c[i][k] * c[k][j];
                }
            }
        }
        return {ans, cnt};
    }
};
