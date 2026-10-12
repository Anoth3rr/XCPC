template<class T> Pt center1(Pt p1, Pt p2, Pt p3) { // 外心
    return lineIntersection(midSegment({p1, p2}), midSegment({p2, p3}));
}
