template<class T> ld disPointToLine(Pt p, Lt l) {
    ld ans = cross(p, l.a, l.b);
    return abs(ans) / dis(l.a, l.b); // 面积除以底边长
}
