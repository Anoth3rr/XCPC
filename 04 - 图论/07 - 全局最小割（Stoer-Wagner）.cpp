/*
用途：无向非负权图的全局最小割，不指定源汇；同时返回割的一侧原点集合。
约定：点编号 0..n-1；重边容量累加。n<2 时割值为 0。
复杂度：O(n^3) 时间、O(n^2) 空间，适合稠密中小图。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/

template <class Cap = int> struct MinCut {
    struct Result {
        Cap val;
        vector<int> sd;
    };

    int n;
    vector<vector<Cap>> w;
    explicit MinCut(int n) : n(n), w(n, vector<Cap>(n)) { assert(n >= 0); }

    void add(int u, int v, Cap c) {
        assert(0 <= u && u < n && 0 <= v && v < n && c >= 0);
        if (u == v) return;
        w[u][v] += c, w[v][u] += c;
    }

    Result solve() const {
        if (n < 2) return {Cap{}, n ? vector<int>{0} : vector<int>{}};
        vector<vector<Cap>> a = w;
        vector<int> v(n);
        iota(v.begin(), v.end(), 0);
        vector<vector<int>> g(n);
        for (int i = 0; i < n; ++i) g[i] = {i};
        Cap ans = numeric_limits<Cap>::max();
        vector<int> sd;

        while (v.size() > 1) {
            vector<Cap> d(n);
            vector<char> use(n);
            int pre = -1, sel = -1;
            for (int k = 0; k < v.size(); ++k) {
                sel = -1;
                for (int x : v) if (!use[x] && (sel == -1 || d[x] > d[sel])) sel = x;
                if (k + 1 == v.size()) {
                    if (d[sel] < ans) ans = d[sel], sd = g[sel];
                    for (int x : v) if (x != pre) a[pre][x] += a[sel][x], a[x][pre] = a[pre][x];
                    g[pre].insert(g[pre].end(), g[sel].begin(), g[sel].end());
                    v.erase(find(v.begin(), v.end(), sel));
                    break;
                }
                use[sel] = true;
                for (int x : v) if (!use[x]) d[x] += a[sel][x];
                pre = sel;
            }
        }
        return {ans, move(sd)};
    }
};
