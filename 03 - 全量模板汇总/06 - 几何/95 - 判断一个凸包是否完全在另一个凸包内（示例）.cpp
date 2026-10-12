int inConvex(const vector<P> &a, P p) {
    int n = a.size();
    if (!n) return 0;
    if (n == 1) return p.x == a[0].x && p.y == a[0].y ? 1 : 0;
    if (n == 2) return onSegment(a[0], a[1], p) ? 1 : 0;
    i128 x = turn(a[0], a[1], p), y = turn(a[0], a[n-1], p);
    if (x < 0 || y > 0) return 0;
    if (!x) return onSegment(a[0], a[1], p) ? 1 : 0;
    if (!y) return onSegment(a[0], a[n-1], p) ? 1 : 0;
    int l = 1, r = n - 1;
    while (l + 1 < r) {
        int m = (l + r) / 2;
        if (turn(a[0], a[m], p) >= 0) l = m;
        else r = m;
    }
    i128 v = turn(a[l], a[r], p);
    return v < 0 ? 0 : v == 0 ? 1 : 2;
}
bool containsHull(const vector<P> &a, const vector<P> &b, bool strict = true) {
    for (auto p : b) if (inConvex(a, p) < (strict ? 2 : 1)) return false;
    return true;
}
