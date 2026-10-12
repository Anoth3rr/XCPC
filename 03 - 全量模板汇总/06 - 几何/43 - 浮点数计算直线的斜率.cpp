template <class T> ld slope(Pt p1, Pt p2) { // 斜率，注意 inf 的情况
    return (p1.y - p2.y) / (p1.x - p2.x);
}
template <class T> ld slope(Lt l) {
    return slope(l.a, l.b);
}
