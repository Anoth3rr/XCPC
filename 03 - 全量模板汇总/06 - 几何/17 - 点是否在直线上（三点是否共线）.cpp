template<class T> bool onLine(Point<T> a, Point<T> b, Point<T> c) {
    return sign(cross(b, a, c)) == 0;
}
template<class T> bool onLine(Point<T> p, Line<T> l) {
    return onLine(p, l.a, l.b);
}
