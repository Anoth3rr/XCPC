using P = Point<i64>;
i128 turn(P a, P b, P c) {
    return i128(b.x - a.x) * (c.y - a.y) - i128(b.y - a.y) * (c.x - a.x);
}
i128 area2(P a, P b, P c) {
    i128 v = turn(a, b, c);
    return v < 0 ? -v : v;
}
bool onSegment(P a, P b, P p) {
    return turn(a, b, p) == 0 && min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x)
        && min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
}
vector<P> convexHullInt(vector<P> a) {
    sort(a.begin(), a.end(), [](P a, P b) { return tie(a.x, a.y) < tie(b.x, b.y); });
    a.erase(unique(a.begin(), a.end(), [](P a, P b) { return a.x == b.x && a.y == b.y; }), a.end());
    int n = a.size();
    if (n <= 1) return a;
    vector<P> h;
    for (auto p : a) {
        while (h.size() >= 2 && turn(h[h.size() - 2], h.back(), p) <= 0) h.pop_back();
        h.push_back(p);
    }
    int k = h.size();
    for (int i = n - 2; i >= 0; --i) {
        while (h.size() > k && turn(h[h.size() - 2], h.back(), a[i]) <= 0) h.pop_back();
        h.push_back(a[i]);
    }
    h.pop_back();
    return h; // 去重、去掉边上中间点，逆时针，不重复首点
}
