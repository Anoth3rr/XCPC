template<class T> bool lineParallel(Lt p1, Lt p2) {
    return sign(cross(p1.a - p1.b, p2.a - p2.b)) == 0;
}
template<class T> bool lineVertical(Lt p1, Lt p2) {
    return sign(dot(p1.a - p1.b, p2.a - p2.b)) == 0;
}
template<class T> bool same(Line<T> l1, Line<T> l2) {
    return lineParallel(Line{l1.a, l2.b}, {l1.b, l2.a}) &&
           lineParallel(Line{l1.a, l2.a}, {l1.b, l2.b}) && lineParallel(l1, l2);
}
