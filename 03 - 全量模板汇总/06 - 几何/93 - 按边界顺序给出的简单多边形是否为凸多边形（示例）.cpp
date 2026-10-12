bool isConvexPolygon(const vector<P> &p, bool strict = true) {
    int n = p.size(), dir = 0;
    if (n < 3) return false;
    for (int i = 0; i < n; i++) {
        i128 v = turn(p[i], p[(i+1)%n], p[(i+2)%n]);
        if (!v) { if (strict) return false; else continue; }
        int s = v > 0 ? 1 : -1;
        if (dir && s != dir) return false;
        dir = s;
    }
    return dir != 0;
}
