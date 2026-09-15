/*
依赖：07 - 平面几何中的 Point。
用途：求平面点集最近点对；返回 {距离平方, 原下标1, 原下标2}，不足两点返回 {INF,-1,-1}。
复杂度：分治 O(n log n)。距离统一转 long double，避免整数坐标作差/平方溢出。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
using ld = long double;

template <class T>
tuple<ld, int, int> close(vector<Point<T>> p) {
    struct N { Point<T> p; int id; };
    vector<N> a;
    for (int i = 0; i < p.size(); ++i) a.push_back({p[i], i});
    sort(a.begin(), a.end(), [](const N& x, const N& y) {
        return x.p.x != y.p.x ? x.p.x < y.p.x : x.p.y < y.p.y;
    });
    ld ans = numeric_limits<ld>::infinity();
    int ax = -1, ay = -1;
    auto upd = [&](const N& x, const N& y) {
        ld dx = ld(x.p.x) - ld(y.p.x);
        ld dy = ld(x.p.y) - ld(y.p.y);
        ld d = dx * dx + dy * dy;
        if (d < ans) ans = d, ax = x.id, ay = y.id;
    };
    auto cy = [](const N& x, const N& y) {
        return x.p.y != y.p.y ? x.p.y < y.p.y : x.p.x < y.p.x;
    };
    auto dfs = [&](auto&& go, int l, int r) -> void {
        if (r - l <= 3) {
            for (int i = l; i < r; ++i) for (int j = i + 1; j < r; ++j) upd(a[i], a[j]);
            sort(a.begin() + l, a.begin() + r, cy);
            return;
        }
        int m = (l + r) >> 1;
        ld mx = ld(a[m].p.x);
        go(go, l, m), go(go, m, r);
        inplace_merge(a.begin() + l, a.begin() + m, a.begin() + r, cy);
        vector<N> q;
        for (int i = l; i < r; ++i) {
            ld dx = ld(a[i].p.x) - mx;
            if (dx * dx >= ans) continue;
            for (int j = q.size(); j--;) {
                ld dy = ld(a[i].p.y) - ld(q[j].p.y);
                if (dy * dy >= ans) break;
                upd(a[i], q[j]);
            }
            q.push_back(a[i]);
        }
    };
    dfs(dfs, 0, a.size());
    return {ans, ax, ay};
}
