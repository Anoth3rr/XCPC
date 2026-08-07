/*
依赖：07 - 平面几何（Point&Line, with Int）。
用途：二维凸包、凸多边形直径、O(log n) 点包含判定。凸包不重复首点并按逆时针排列。
整数坐标时 T 应能容纳叉积和距离平方；坐标接近 1e9 时通常用 int。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <class T>
vector<Point<T>> convex(vector<Point<T>> a, bool kp = false) {
    sort(a.begin(), a.end(), [](const auto& p, const auto& q) {
        return p.x != q.x ? p.x < q.x : p.y < q.y;
    });
    a.erase(unique(a.begin(), a.end()), a.end());
    int n = a.size();
    if (n <= 2) return a;
    bool col = true;
    for (int i = 2; i < n; ++i) col &= cross(a[1] - a[0], a[i] - a[0]) == 0;
    if (kp && col) return a;

    auto ch = [&](auto fst, auto lst) {
        vector<Point<T>> h;
        for (; fst != lst; ++fst) {
            const auto& p = *fst;
            while (h.size() >= 2) {
                T c = cross(h.back() - h[h.size() - 2], p - h.back());
                if (kp ? c >= 0 : c > 0) break;
                h.pop_back();
            }
            h.push_back(p);
        }
        return h;
    };
    auto lo = ch(a.begin(), a.end());
    auto up = ch(a.rbegin(), a.rend());
    lo.pop_back(), up.pop_back();
    lo.insert(lo.end(), up.begin(), up.end());
    return lo;
}

template <class T> T area2(const vector<Point<T>>& p) {
    T r{};
    for (int i = 0, n = p.size(); i < n; ++i) r += cross(p[i], p[(i + 1) % n]);
    return r;
}

// 输入为逆时针凸多边形；返回 {最大距离平方, 端点下标}。
template <class T>
tuple<T, int, int> diam2(const vector<Point<T>>& p) {
    int n = p.size();
    if (!n) return {T{}, -1, -1};
    if (n == 1) return {T{}, 0, 0};
    auto ab = [](T x) { return x < 0 ? -x : x; };
    T ans{};
    int x = 0, y = 1, j = 1;
    auto upd = [&](int u, int v) {
        T d = square(p[u] - p[v]);
        if (d > ans) ans = d, x = u, y = v;
    };
    for (int i = 0; i < n; ++i) {
        int k = (i + 1) % n;
        while (ab(cross(p[k] - p[i], p[(j + 1) % n] - p[i])) >
               ab(cross(p[k] - p[i], p[j] - p[i]))) j = (j + 1) % n;
        upd(i, j), upd(k, j);
    }
    return {ans, x, y};
}

// -1：外部，0：边界，1：内部；要求 p 为逆时针凸多边形且无重复首点。
template <class T> int inConv(const vector<Point<T>>& p, const Point<T>& q) {
    int n = p.size();
    auto on = [&](const Point<T>& a, const Point<T>& b) {
        return cross(q - a, b - a) == 0 && dot(q - a, q - b) <= 0;
    };
    if (!n) return -1;
    if (n == 1) return q == p[0] ? 0 : -1;
    if (n == 2) return on(p[0], p[1]) ? 0 : -1;
    T a = cross(p[1] - p[0], q - p[0]);
    T b = cross(p[n - 1] - p[0], q - p[0]);
    if (a < 0 || b > 0) return -1;
    if (!a) return on(p[0], p[1]) ? 0 : -1;
    if (!b) return on(p[0], p[n - 1]) ? 0 : -1;
    int l = 1, r = n - 1;
    while (r - l > 1) {
        int m = (l + r) >> 1;
        if (cross(p[m] - p[0], q - p[0]) >= 0) l = m;
        else r = m;
    }
    T c = cross(p[r] - p[l], q - p[l]);
    return c < 0 ? -1 : c == 0 ? 0 : 1;
}
