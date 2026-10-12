template<class T>
bool segmentInPolygon(Line<T> l, vector<Point<T>> p) {
// 线段与多边形边界不相交且两端点都在多边形内部
#define L(x, y) pointOnLineLeft(x, y)
    int n = p.size();
    if (!pointInPolygon(l.a, p)) return false;
    if (!pointInPolygon(l.b, p)) return false;
    for (int i = 0; i < n; i++) {
        auto u = p[i];
        auto v = p[(i + 1) % n];
        auto w = p[(i + 2) % n];
        auto [t, p1, p2] = segmentIntersection(l, Line(u, v));
        if (t == 1) return false;
        if (t == 0) continue;
        if (t == 2) {
            if (pointOnSegment(v, l) && v != l.a && v != l.b) {
                if (cross(v - u, w - v) > 0) {
                    return false;
                }
            }
        } else {
            if (p1 != u && p1 != v) {
                if (L(l.a, Line(v, u)) || L(l.b, Line(v, u))) {
                    return false;
                }
            } else if (p1 == v) {
                if (l.a == v) {
                    if (L(u, l)) {
                        if (L(w, l) && L(w, Line(u, v))) {
                            return false;
                        }
                    } else {
                        if (L(w, l) || L(w, Line(u, v))) {
                            return false;
                        }
                    }
                } else if (l.b == v) {
                    if (L(u, Line(l.b, l.a))) {
                        if (L(w, Line(l.b, l.a)) && L(w, Line(u, v))) {
                            return false;
                        }
                    } else {
                        if (L(w, Line(l.b, l.a)) || L(w, Line(u, v))) {
                            return false;
                        }
                    }
                } else {
                    if (L(u, l)) {
                        if (L(w, Line(l.b, l.a)) || L(w, Line(u, v))) {
                            return false;
                        }
                    } else {
                        if (L(w, l) || L(w, Line(u, v))) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}
