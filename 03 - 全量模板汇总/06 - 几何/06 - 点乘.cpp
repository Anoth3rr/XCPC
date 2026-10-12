template<class T> T dot(Point<T> a, Point<T> b) { // 点乘
    return a.x * b.x + a.y * b.y;
}
template<class T> T dot(Point<T> p1, Point<T> p2, Point<T> p0) { // 点乘 (p1 - p0) * (p2 - p0);
    return dot(p1 - p0, p2 - p0);
}
