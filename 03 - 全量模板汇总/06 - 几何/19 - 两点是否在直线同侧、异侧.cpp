template<class T> bool pointOnLineSide(Pt p1, Pt p2, Lt vec) {
    T val = cross(p1, vec.a, vec.b) * cross(p2, vec.a, vec.b);
    return sign(val) == 1;
}
template<class T> bool pointNotOnLineSide(Pt p1, Pt p2, Lt vec) {
    T val = cross(p1, vec.a, vec.b) * cross(p2, vec.a, vec.b);
    return sign(val) == -1;
}
