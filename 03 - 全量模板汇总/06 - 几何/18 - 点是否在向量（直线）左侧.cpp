template<class T> bool pointOnLineLeft(Pt p, Lt l) {
    return cross(l.b, p, l.a) > 0;
}
