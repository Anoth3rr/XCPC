template<class T> T areaEx(Point<T> p1, Point<T> p2, Point<T> p3) {
    return cross(p2, p3, p1);
}
