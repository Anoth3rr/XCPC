/*
依赖：07 - 平面几何、07A - 凸包。
用途：求两个点集凸包的闵可夫斯基和；结果为逆时针凸包，不重复首点、去共线中间点。
复杂度：含两次凸包为 O(n log n+m log m)，已给凸包时主体为 O(n+m)。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <class T>
vector<Point<T>> minkSum(vector<Point<T>> a, vector<Point<T>> b) {
    a = convex(move(a)), b = convex(move(b));
    if (a.empty() || b.empty()) return {};
    if (a.size() == 1) {
        for (auto& p : b) p += a[0];
        return b;
    }
    if (b.size() == 1) {
        for (auto& p : a) p += b[0];
        return a;
    }
    auto z = [](vector<Point<T>>& p) {
        auto it = min_element(p.begin(), p.end(), [](const auto& x, const auto& y) {
            return x.y != y.y ? x.y < y.y : x.x < y.x;
        });
        rotate(p.begin(), it, p.end());
    };
    z(a), z(b);
    vector<Point<T>> x(a.size()), y(b.size());
    for (int i = 0; i < a.size(); ++i) x[i] = a[(i + 1) % a.size()] - a[i];
    for (int i = 0; i < b.size(); ++i) y[i] = b[(i + 1) % b.size()] - b[i];

    int i = 0, j = 0;
    Point<T> p = a[0] + b[0];
    vector<Point<T>> c{p};
    while (i < x.size() || j < y.size()) {
        Point<T> d;
        if (j == y.size() || (i < x.size() && cross(x[i], y[j]) > 0)) d = x[i++];
        else if (i == x.size() || cross(x[i], y[j]) < 0) d = y[j++];
        else d = x[i++] + y[j++];
        p += d, c.push_back(p);
    }
    c.pop_back();
    return convex(move(c));
}
